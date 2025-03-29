#include "QtWidgetsApplication1.h"
#include "Reader.h"
#include <QtWidgets/QApplication>

extern AccountReader* cur_account;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    cur_account = new AccountReader;

    QtWidgetsApplication1 w;
    w.show();
    return a.exec();
}
