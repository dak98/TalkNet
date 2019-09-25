#include "cli_handlers.hpp"

namespace talk_net
{

auto cli_SEND(QString arguments, socket_io::client& client_handle) -> QString
{
    try
    {
        client_handle.send(arguments.toStdString());
    }
    catch (std::exception const& e)
    {
        return "An error occured: " + QString(e.what());
    }
    return QString();
}

}
