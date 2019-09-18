#include <QApplication>
#include <QHBoxLayout>
#include <QScopedPointer>
#include <QTextEdit>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget main_window;
    main_window.setWindowTitle("Server");

    QHBoxLayout main_layout;

    QTextEdit messages_from_clients, user_console;
    messages_from_clients.setReadOnly(true);

    /*
     * QWidget object takes ownership of the QHBoxLayout object which takes
     * ownership of the QTextEdit objects.
     */
    main_layout.addWidget(&messages_from_clients);
    main_layout.addWidget(&user_console);
    main_window.setLayout(&main_layout);

    main_window.show();



    return a.exec();
}
