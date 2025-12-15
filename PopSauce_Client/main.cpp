#include "widgetpopsauceclient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WidgetPopSauceClient w;
    w.show();
    return a.exec();
}
