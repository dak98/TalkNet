#include <QApplication>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QWidget>

#include <iostream>
#include <thread>

#include <socket_io/client.hpp>

#include "QConsole.hpp"
#include "utility.hpp"

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

    /*
     * QWidget object takes ownership of the QHBoxLayout object which takes
     * ownership of the QTextEdit and QLineEdit objects.
     */
    main_layout.addWidget(&received_messages);
    main_layout.addWidget(&user_console);
    main_window.setLayout(&main_layout);

    socket_io::ipv4_socket_address address_of_client{argv[1], argv[2]};
    socket_io::client client_handle{address_of_client};
    main_window.show();

    std::thread collector_tid{talk_net::collect_messages<socket_io::client>,
                              std::ref(client_handle),
                              std::ref(received_messages)};
    collector_tid.detach();
    /*
     * The closing of the client's collector thread should happen just before
     * the destruction of the server and QTextEdit objects.
     */
    QObject::connect(&app, &QApplication::aboutToQuit,
                     [&collector_tid]()
                     {
                         pthread_cancel(collector_tid.native_handle());
                     });

    return app.exec();
}
