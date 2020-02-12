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

#ifndef TALKNET_UTILITY_HPP_
#define TALKNET_UTILITY_HPP_

#include <QHash>
#include <QString>
#include <QTextEdit>

#include <string>

#include <iostream>

/*
 * This file contains functions used by both the client and the server. The
 * main goal here is to avoid code repetition and NOT to reuse them in other
 * areas.
 */
namespace talk_net
{

/*
 * The first argument of a cli_handler are the arguments given to the command
 * or QString() (empty) if there aren't any.
 */
template<class handle_type>
using cli_handler = std::function<QString(QString, handle_type&)>;
template<class handle_type>
extern QHash<QString, cli_handler<handle_type>> cli_handlers;

/*
 * Being run in a separate thread, this function waits for any messages
 * received, displays them using the QTextEdit object and runs any needed
 * handlers.
 *
 * Care must be taken to ensure that the lifespan of this function does not
 * exceed the lifespan of its arguments.
 */
template<class handle_type>
inline auto handle_messages(handle_type& handle, QTextEdit& output) -> void
{
    bool is_server_working = true;
    while (is_server_working)
    try
    {        
        QString const message = handle.receive().c_str();
        int const pos_of_arguments = message.indexOf(' ');
        QString const command = message.mid(0, pos_of_arguments);
        QString const arguments = pos_of_arguments == -1
                                  ? QString()
                                  : message.mid(pos_of_arguments + 1, -1);
        if (message == "EXIT")
            is_server_working = false;
        else
            output.append(arguments);

        if (cli_handlers<handle_type>.contains(command))
            cli_handlers<handle_type>[command](arguments, handle);
    }
    catch (std::system_error const&)
    {
        is_server_working = false;
    }
    /*
     * For a reason unknown to me, writing this message to QTextEdit causes a
     * segmentation fault.
     */
    std::cout << "The connection to the server has been closed." << std::endl;
}

/*
 * Care must be taken to ensure that the lifespan of this function does not
 * exceed the lifespan of its arguments.
 */
template<class command_type, class handle_type>
inline auto dispatch_cli_command(QString command, QString arguments,
                                 QHash<QString, command_type> const& commands,
                                 handle_type& handle) -> QString
{
    QString result;
    if (commands.contains(command))
        result = commands[command](arguments, handle);
    else
        result = command + ": Unknown command";
    return result;
}

}

#endif // TALKNET_UTILITY_HPP_
