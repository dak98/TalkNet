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
    QString const id = QString::number(client_handle.get_id());
    return send("ECHO " + id + " " + arguments, client_handle);
}

auto cli_INFO(QString arguments, socket_io::client& client_handle) -> QString
{
    return "ID=" + QString::number(client_handle.get_id());
}

auto cli_SEND(QString arguments, socket_io::client& client_handle) -> QString
{
    return send("SEND " + arguments, client_handle);
}

auto cli_SENDTO(QString arguments, socket_io::client& client_handle)
    -> QString
{
    int const message_start_pos = arguments.indexOf(' ');
    if (message_start_pos == -1)
        return "Not enough arguments. Should be SENDTO [client's id] [message]";
    return send("SEND " + arguments, client_handle);
}

}
