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

#include "cli_handlers.hpp"

namespace talk_net
{

inline auto send(QString message, socket_io::client& client_handle) -> QString
{
    try
    {
        client_handle.send(message.toStdString());
    }
    catch (std::exception const& e)
    {
        return "An error occured: " + QString(e.what());
    }
    return QString();
}

auto cli_ECHO(QString arguments, socket_io::client& client_handle) -> QString
{
    QString const self_id_str = QString::number(client_handle.get_id());
    return cli_SENDTO(self_id_str + " " + arguments, client_handle);
}

auto cli_INFO(QString arguments, socket_io::client& client_handle) -> QString
{
    return "ID=" + QString::number(client_handle.get_id());
}

auto cli_SENDTO(QString arguments, socket_io::client& client_handle)
    -> QString
{
    int const message_start_pos = arguments.indexOf(' ');
    if (message_start_pos == -1)
        return "Not enough arguments. Should be SENDTO [client's id] [message]";

    QString const id_str = arguments.mid(0, message_start_pos);
    arguments = arguments.mid(message_start_pos + 1, -1);
    QString const self_id_str = QString::number(client_handle.get_id());

    return send("SEND " + id_str + " [CLIENT " + self_id_str + "] " +
                arguments, client_handle);
}

}
