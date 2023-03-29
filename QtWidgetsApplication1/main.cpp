#include "QtWidgetsApplication1.h"
#include <QtWidgets/QApplication>
#include <QApplication>
#include <QMainWindow>
/*#include "image.h"
#include "imagewindow.h"
#include "drawlinecommand.h"
#include "commandhistory.h"
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QtWidgetsApplication1 w;
    w.show();
    return a.exec();
}
