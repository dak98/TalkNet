#include <QApplication>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QWidget>

#include <iostream>

#include "QConsole.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Expected 2 arguments, got " + std::to_string(argc)
                  << ". Should be [Server's address] [Server's port]"
                  << std::endl;
        return -1;
    }

    QApplication app(argc, argv);

    QWidget main_window;
    main_window.setWindowTitle("Client");
    QHBoxLayout main_layout;

    QTextEdit received_messages;
    received_messages.setReadOnly(true);
    QConsole user_console;

    /*
     * QWidget object takes ownership of the QHBoxLayout object which takes
     * ownership of the QTextEdit and QLineEdit objects.
     */
    main_layout.addWidget(&received_messages);
    main_layout.addWidget(&user_console);
    main_window.setLayout(&main_layout);

    main_window.show();

    return app.exec();
}
