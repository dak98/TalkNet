#include "cli_handlers.hpp"

namespace talk_net
{

QString cli_LIST(QString arguments, socket_io::server& server_handle)
{
    auto connected_clients = server_handle.get_connected_clients();
    QString output;
    for (auto const& [id, client_view] : connected_clients)
        output += QString::number(id) + ": " +
                  QString::fromStdString(client_view.to_string()) + "\n";
    return output;
}

}
