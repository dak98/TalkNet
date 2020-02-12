/*
  Copyright 2019 Jakub Czajka

  This file is part of TalkNet.

  TalkNet is free software: you can redistribute it and/or modify it under the
  terms of the GNU Lesser General Public License as published by the Free
  Software Foundation, either version 3 of the License, or (at your option) any
  later version.

  TalkNet is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public License
  along with TalkNet. If not, see <https://www.gnu.org/licenses/>.
*/

#include "QConsole.hpp"

#include <iostream>

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
    if (output != QString())
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
