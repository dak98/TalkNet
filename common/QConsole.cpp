#include "QConsole.hpp"

QConsole::QConsole(QWidget *parent)
    : QWidget(parent)
{
    connect(&user_input, &QLineEdit::returnPressed,
            this, &QConsole::inputReceived);

    main_layout.addWidget(&commands_output);
    main_layout.addWidget(&user_input);

    setLayout(&main_layout);

    unfreeze();
}

void QConsole::freeze()
{
    user_input.setPlaceholderText(QString());
    user_input.setReadOnly(true);
}

void QConsole::unfreeze()
{
    user_input.setPlaceholderText("Enter a command...");
    user_input.setReadOnly(false);
}

void QConsole::setCommandOutput(const QString& command, const QString& output)
{
    commands_output.append(command + ": " + output);
    unfreeze();
}

void QConsole::inputReceived()
{
    freeze();
    const QString input = user_input.text();
    user_input.clear();

    const int pos_of_arguments = input.indexOf(' ');    
    const QString command = input.mid(0, pos_of_arguments);
    const QString arguments = pos_of_arguments == -1
                              ? QString()
                              : input.mid(pos_of_arguments + 1, -1);
    if (registered_commands.contains(command))
        emit commandReceived(command, arguments);
    else
        setCommandOutput(command, "Unknown command");
}
