/*
  Copyright 2019 Jakub Czajka

  This file is part of TalkNet.

  TalkNet is free software: you can redistribute it and/or modify it under the
  terms of the GNU Lesser General Public License as published by the Free
  Software Foundation, either version 3 of the License, or (at your option) any
  later version.

  TalkNet is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public License
  along with TalkNet. If not, see <https://www.gnu.org/licenses/>.
*/

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

namespace talk_net
{

template<>
QHash<QString, cli_handler<socket_io::client>> cli_handlers<socket_io::client>
{
    {"ECHO", talk_net::cli_ECHO},
    {"INFO", talk_net::cli_INFO},
    {"SENDTO", talk_net::cli_SENDTO}
};

} // talk_net

/*
 * To avoid redundancy, some of the comments which also appear in the server
 * program were omitted from here. Check server.cpp for further explanation
 * on the code.
 */
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Expected 2 arguments, got " + std::to_string(argc - 1)
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

    for (auto it = talk_net::cli_handlers<socket_io::client>.begin();
              it != talk_net::cli_handlers<socket_io::client>.end(); ++it)
        user_console.registerCommand(it.key());

    main_layout.addWidget(&received_messages);
    main_layout.addWidget(&user_console);
    main_window.setLayout(&main_layout);

    socket_io::ipv4_socket_address address_of_client{argv[1], argv[2]};
    socket_io::client client_handle{address_of_client};

    std::thread collector_tid{talk_net::handle_messages<socket_io::client>,
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
                         using namespace socket_io;

                         QString output =
                                 dispatch_cli_command(command,
                                                      arguments,
                                                      cli_handlers<client>,
                                                      client_handle);
                         user_console.setCommandOutput(command, output);
                     });

    main_window.show();
    return app.exec();
}
