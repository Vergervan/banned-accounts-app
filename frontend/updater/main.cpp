#include "widget.h"

#include <QApplication>
#include <QColor>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    QPalette pal;
    pal.setColor(QPalette::Window, QColor(54, 54, 54));
    w.setAutoFillBackground(true);
    w.setPalette(pal);
    w.show();
    return a.exec();
}
