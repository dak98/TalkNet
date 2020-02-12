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

#ifndef QCONSOLE_HPP
#define QCONSOLE_HPP

#include <QLineEdit>
#include <QSet>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

/*
 * A widget for a simple command-line interface. It consist of two panels:
 * 1) QLineEdit for receiving the input from the user.
 * 2) QTextEdit for displaying the output of the commands.
 *
 * Available commands should be registered in the class. Otherwise, a message
 * about a unknown command will be displayed.
 *
 * When a known command is entered, the commandReceived signal is raised. The
 * user should connect to it, handle the input and provide the output to be
 * displayed through setCommandOuput. Command input is disabled while QConsole
 * waits for the result.
 */
class QConsole : public QWidget
{
    Q_OBJECT

    void freeze();
    void unfreeze();
public:
    explicit QConsole(QWidget *parent = nullptr);

    void registerCommand(const QString &command)
    { registered_commands.insert(command); }

signals:
    // If no arguments were provided, arguments is set to QString() (empty).
    void commandReceived(QString command, QString arguments);

public slots:
    void setCommandOutput(const QString &command, const QString &output);

private slots:
    void inputReceived();

private:
    QVBoxLayout main_layout;

    QLineEdit user_input;
    QTextEdit commands_output;

    QSet<QString> registered_commands;
};

#endif // QCONSOLE_HPP
