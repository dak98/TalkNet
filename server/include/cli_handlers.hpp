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

#ifndef TALKNET_CLI_HANDLERS_H_
#define TALKNET_CLI_HANDLERS_H_

#include <QString>

#include <socket_io/server.hpp>

namespace talk_net
{

auto cli_LIST(QString arguments, socket_io::server& server_handle) -> QString;
auto cli_SEND(QString arguments, socket_io::server& server_handle) -> QString;

}

#endif // TALKNET_CLI_HANDLERS_H_
