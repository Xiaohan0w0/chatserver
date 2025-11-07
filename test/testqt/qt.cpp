// simple_test.cpp
#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLabel label("Qt环境工作正常！");
    label.show();
    return app.exec();
}