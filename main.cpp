#include "windowfsk.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WindowFSK w;
    w.show();

    return a.exec();
}
