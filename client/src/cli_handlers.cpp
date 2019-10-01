#include "cli_handlers.hpp"

namespace talk_net
{

auto cli_ECHO(QString arguments, socket_io::client& client_handle) -> QString
{
    QString const id = QString::number(client_handle.get_id());
    try
    {
        client_handle.send("ECHO " + id.toStdString() + " " +
                           arguments.toStdString());
    }
    catch (std::exception const& e)
    {
        return "An error occured: " + QString(e.what());
    }
    return QString();
}

auto cli_SEND(QString arguments, socket_io::client& client_handle) -> QString
{
    try
    {
        client_handle.send("SEND " + arguments.toStdString());
    }
    catch (std::exception const& e)
    {
        return "An error occured: " + QString(e.what());
    }
    return QString();
}

}
