#ifndef TALKNET_CLI_HANDLERS_H_
#define TALKNET_CLI_HANDLERS_H_

#include <QString>

#include <socket_io/server.hpp>

namespace talk_net
{

QString cli_LIST(QString arguments, socket_io::server& server_handle);

}

#endif // TALKNET_CLI_HANDLERS_H_
