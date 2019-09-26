#include <QApplication>
#include <QHash>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QWidget>

#include <iostream>
#include <thread>

#include <socket_io/client.hpp>

#include "cli_handlers.hpp"
#include "QConsole.hpp"
#include "utility.hpp"

/*
 * The first argument of a cli_handler are the arguments given to the command
 * or QString() (empty) if there aren't any.
 */
using cli_handler = std::function<QString(QString, socket_io::client&)>;
inline QHash<QString, cli_handler> cli_handlers
{
    {"SEND", talk_net::cli_SEND}
};

/*
 * To avoid redundancy, some of the comments which also appear in the server
 * program were omitted from here. Check server.cpp for further explanation
 * on the code.
 */
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Expected 2 arguments, got " + std::to_string(argc)
                  << ". Should be [Server's address] [Server's port]"
                  << std::endl;
        return -1;
    }

    QApplication app(argc, argv);

    QWidget main_window;
    main_window.setWindowTitle("Client");
    QHBoxLayout main_layout;

    QTextEdit received_messages;
    received_messages.setReadOnly(true);
    QConsole user_console;

    for (auto it = cli_handlers.begin(); it != cli_handlers.end(); ++it)
        user_console.registerCommand(it.key());

    main_layout.addWidget(&received_messages);
    main_layout.addWidget(&user_console);
    main_window.setLayout(&main_layout);

    socket_io::ipv4_socket_address address_of_client{argv[1], argv[2]};
    socket_io::client client_handle{address_of_client};

    std::thread collector_tid{talk_net::collect_messages<socket_io::client>,
                              std::ref(client_handle),
                              std::ref(received_messages)};

    qRegisterMetaType<QTextCursor>("QTextCursor");
    QObject::connect(&app, &QApplication::aboutToQuit,
                     [&collector_tid]()
                     {
                         if (collector_tid.joinable())
                             collector_tid.join();
                         else
                            pthread_cancel(collector_tid.native_handle());
                     });

    QObject::connect(&user_console, &QConsole::commandReceived,
                     [&client_handle, &user_console]
                     (QString command, QString arguments)
                     {
                         using namespace talk_net;
                         QString output = dispatch_cli_command(command,
                                                               arguments,
                                                               cli_handlers,
                                                               client_handle);
                         user_console.setCommandOutput(command, output);
                     });

    main_window.show();
    return app.exec();
}
