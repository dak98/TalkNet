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

auto cli_LIST(QString, socket_io::server& server_handle) -> QString
{
    auto connected_clients = server_handle.get_connected_clients();
    QString output = "\n";
    for (auto const& [id, client_view] : connected_clients)
        output += QString::number(id) + ": " +
                  QString::fromStdString(client_view.to_string()) + "\n";
    return output;
}

auto cli_SEND(QString arguments, socket_io::server& server_handle) -> QString
{
    int const message_start_pos = arguments.indexOf(' ');
    if (message_start_pos == -1)
        return "Not enough arguments. Should be SEND [client's id] [message]";

    QString const id_str = arguments.mid(0, message_start_pos);

    bool result;
    int const id = id_str.toInt(&result);
    if (!id)
        return "Client's id must be an integer";

    arguments = arguments.mid(message_start_pos + 1, -1);
    try
    {
        server_handle.send(id, "SEND " + arguments.toStdString());
    }
    catch (std::out_of_range const&)
    {
        return "Client with id " + id_str + " does not exist.";
    }
    return QString();
}

}
