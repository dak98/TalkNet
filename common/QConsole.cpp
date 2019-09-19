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

    const QString command = input.mid(0, input.indexOf(' '));
    if (registered_commands.contains(command))
        emit commandReceived(input);
    else
        setCommandOutput(command, "Unknown command");
}
