#include <QApplication>
#include <QHash>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QWidget>

#include <functional>
#include <iostream>
#include <thread>

#include <socket_io/protocols.hpp>
#include <socket_io/server.hpp>

#include "cli_handlers.hpp"
#include "QConsole.hpp"
#include "utility.hpp"

/*
 * The first argument of a cli_handler are the arguments given to the command
 * or QString() (empty) if there aren't any.
 */
using cli_handler = std::function<QString(QString, socket_io::server&)>;
inline QHash<QString, cli_handler> cli_handlers
{
    {"LIST", talk_net::cli_LIST}
};

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Expected 1 argument but got " + std::to_string(argc)
                  << ". Should be [Server's port]" << std::endl;
        return -1;
    }

    QApplication app(argc, argv);

    QWidget main_window;
    main_window.setWindowTitle("Server");
    QHBoxLayout main_layout;

    QTextEdit messages_from_clients;
    messages_from_clients.setReadOnly(true);
    QConsole user_console;

    for (auto it = cli_handlers.begin(); it != cli_handlers.end(); ++it)
        user_console.registerCommand(it.key());

    /*
     * QWidget object takes ownership of the QHBoxLayout object which takes
     * ownership of the QTextEdit and QLineEdit objects.
     */
    main_layout.addWidget(&messages_from_clients);
    main_layout.addWidget(&user_console);
    main_window.setLayout(&main_layout);

    socket_io::server server_handle{argv[1], socket_io::ip_protocol::IPv4};

    /*
     * std::thread is used here instead of QThread as functions perform
     * operations on socket_io objects. A Qt-based wrapper would (probably)
     * need to be written which would defeat some of the purpose of using the
     * library in the first place.
     */
    std::thread collector_tid{talk_net::collect_messages<socket_io::server>,
                              std::ref(server_handle),
                              std::ref(messages_from_clients)};
    collector_tid.detach();

    // The signals and slots mechanism requires this line.
    qRegisterMetaType<QTextCursor>("QTextCursor");
    /*
     * The closing of the server's collector thread should happen just before
     * the destruction of the server and QTextEdit objects.
     */    
    QObject::connect(&app, &QApplication::aboutToQuit,
                     [&collector_tid]()
                     {
                         pthread_cancel(collector_tid.native_handle());
                     });

    QObject::connect(&user_console, &QConsole::commandReceived,
                     [&server_handle, &user_console]
                     (QString command, QString arguments)
                     {
                         using namespace talk_net;
                         QString output = dispatch_cli_command(command,
                                                               arguments,
                                                               cli_handlers,
                                                               server_handle);
                         user_console.setCommandOutput(command, output);
                     });

    main_window.show();
    return app.exec();
}
