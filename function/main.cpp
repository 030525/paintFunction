#include "canvas.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    canvas w;
    w.show();
    return a.exec();
}
