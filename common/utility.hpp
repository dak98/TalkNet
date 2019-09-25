#ifndef TALKNET_UTILITY_HPP_
#define TALKNET_UTILITY_HPP_

#include <QHash>
#include <QString>
#include <QTextEdit>

#include <string>

/*
 * This file contains functions used by both the client and the server. The
 * main goal here is to avoid code repetition and NOT to reuse them in other
 * areas.
 */
namespace talk_net
{

/*
 * Being run in a separate thread, this function waits for any messages sent
 * to the handle and displays them using the QTextEdit object.
 *
 * Care must be taken to ensure that the lifespan of this function does not
 * exceed the lifespan of its arguments.
 */
template<class handle_type>
inline auto collect_messages(handle_type& handle, QTextEdit& output) -> void
try
{
    for (;;)
    {
        std::string const message = handle.receive();
        output.append(message.c_str());
    }
}
catch (std::system_error const&)
{ /* Ignore this error */ }

/*
 * Care must be taken to ensure that the lifespan of this function does not
 * exceed the lifespan of its arguments.
 */
template<class command_type, class handle_type>
inline auto dispatch_cli_command(QString command, QString arguments,
                                 QHash<QString, command_type> const& commands,
                                 handle_type& handle) -> QString
{
    QString result;
    if (commands.contains(command))
        result = commands[command](arguments, handle);
    else
        result = command + ": Unknown command";
    return result;
}

}

#endif // TALKNET_UTILITY_HPP_
