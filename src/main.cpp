#include "mainwindow.h"

// #include "projectnavigator.h"
// #include "taskview.h"
// #include "infowidget.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // w.show();
    // ProjectNavigator::instance();
    // TaskView::instance();
    // InfoWidget::instance();
    MainWindow::instance()->show();
    return a.exec();
}
