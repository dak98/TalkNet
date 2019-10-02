#include "cli_handlers.hpp"

namespace talk_net
{

auto cli_ECHO(QString arguments, socket_io::server& server_handle) -> QString
{
    return cli_SEND(arguments, server_handle);
}

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
