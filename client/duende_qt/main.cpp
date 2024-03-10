#include "duende.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Duende w;
    w.show();
    return a.exec();
}
