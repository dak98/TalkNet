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
