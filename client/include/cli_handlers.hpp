#ifndef TALKNET_CLI_HANDLERS_HPP_
#define TALKNET_CLI_HANDLERS_HPP_

#include <QString>

#include <socket_io/client.hpp>

namespace talk_net
{

auto cli_ECHO(QString arguments, socket_io::client& client_handle) -> QString;
auto cli_INFO(QString arguments, socket_io::client& client_handle) -> QString;
auto cli_LIST(QString arguments, socket_io::client& client_handle) -> QString;
auto cli_SENDTO(QString arguments, socket_io::client& client_handle)
    -> QString;

}

#endif // TALKNET_CLI_HANDLERS_HPP
