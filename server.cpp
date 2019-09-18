#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget main_window;
    main_window.setWindowTitle("Server");

    main_window.show();
    return a.exec();
}
