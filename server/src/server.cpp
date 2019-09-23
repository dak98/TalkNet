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

/*
 * Being run in a separate thread, this function waits for any messages from
 * the clients and displays them using the QTextEdit object.
 */
[[noreturn]]
static void collect_messages_from_clients(socket_io::server& server_handle,
                                          QTextEdit& messages_from_clients)
{
    for (;;)
    {
        std::string const message = server_handle.receive();
        messages_from_clients.append(message.c_str());
    }
}

/*
 * The first argument of a cli_handler are the arguments given to the command
 * or QString() (empty) if there aren't any.
 */
using cli_handler = std::function<QString(QString, socket_io::server&)>;
inline QHash<QString, cli_handler> cli_handlers
{
    {"LIST", talk_net::cli_LIST}
};

static QString dispatch_cli_command(QString command,
                                    QString arguments,
                                    socket_io::server& server_handle)
{
    QString result;
    if (cli_handlers.contains(command))
        result = cli_handlers[command](arguments, server_handle);
    else
        result = command + ": Unknown command";
    return result;
}

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
    main_window.show();

    std::thread collector_tid = std::thread{collect_messages_from_clients,
                                            std::ref(server_handle),
                                            std::ref(messages_from_clients)};
    collector_tid.detach();
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
                         QString output = dispatch_cli_command(command,
                                                               arguments,
                                                               server_handle);
                         user_console.setCommandOutput(command, output);
                     });

    return app.exec();
}
