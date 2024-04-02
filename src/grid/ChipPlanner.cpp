#include "ChipPlanner.h"

ChipPlanner::ChipPlanner(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(1280, 720);
    menuBar = new QMenuBar(this);
    this->setMenuBar(menuBar);
    fileMenu = menuBar->addMenu("File");
    editMenu = menuBar->addMenu("Edit");

    toolbar = new QToolBar("ToolBar", this);
    toolbar->setOrientation(Qt::Vertical);
    addToolBar(Qt::LeftToolBarArea, toolbar);
    QAction *a1 = new QAction("A1", this);
    QAction *a2 = new QAction("A2", this);
    QAction *a3 = new QAction("A3", this);
    QAction *a4 = new QAction("A4", this);
    QAction *a5 = new QAction("A5", this);
    toolbar->addAction(a1);
    toolbar->addAction(a2);
    toolbar->addAction(a3);
    toolbar->addAction(a4);
    toolbar->addAction(a5);
}

ChipPlanner::~ChipPlanner()
{

}
