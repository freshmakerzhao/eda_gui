#include "Form.h"
#include "mainwindow.h"

Form *Form::instance()
{
    static Form *_instance = nullptr;
    if (!_instance) {
        _instance = new Form(nullptr);
    }
    return _instance;
}

Form::Form(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("QWidget { background-image: url(:/resource/white.png); }"
                  "QGroupBox { font-family: Console; font-size: 30px; }"
                  "QPushButton { border: none; text-align: left; font-size: 22px; }"
                  );
    QPixmap logoPixmap(":/resource/logo1.png");
    QLabel *logoLabel = new QLabel(this);
    logoLabel->setPixmap(logoPixmap.scaled(280, 50));
    logoLabel->setFixedHeight(50);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(logoLabel, 0, Qt::AlignLeft);
    layout->setMargin(30);

    QGroupBox *groupBox = new QGroupBox("Quick Start");
    QVBoxLayout *groupLayout = new QVBoxLayout(groupBox);
    QPushButton *button1 = new QPushButton("Create Project >");
    connect(button1, &QPushButton::clicked, MainWindow::instance(), &MainWindow::onNewTriggered);
    // button1->QPushButton::setFlat(true);
    // button1->setStyleSheet( "QPushButton:hover {color: red;");
    // button1->setStyleSheet("QPushButton { font-family: Times New Roman; font-size: 14px; }");
    QPushButton *button2 = new QPushButton("Open Project >");
    connect(button2, &QPushButton::clicked, MainWindow::instance(), &MainWindow::onOpenTriggered);
    QPushButton *button3 = new QPushButton("Open Example Project >");
    button3->setEnabled(false);
    groupLayout->addWidget(button1);
    groupLayout->addWidget(button2);
    groupLayout->addWidget(button3);
    layout->addWidget(groupBox);

    QGroupBox *groupBox1 = new QGroupBox("Tasks");
    QVBoxLayout *groupLayout1 = new QVBoxLayout(groupBox1);
    QPushButton *button4 = new QPushButton("Manage IP >");
    QPushButton *button5 = new QPushButton("Open Hardware Manager >");
    QPushButton *button6 = new QPushButton("XHub Store >");
    button4->setEnabled(false);
    button5->setEnabled(false);
    button6->setEnabled(false);
    groupLayout1->addWidget(button4);
    groupLayout1->addWidget(button5);
    groupLayout1->addWidget(button6);
    layout->addWidget(groupBox1);

    QGroupBox *groupBox2 = new QGroupBox("Learning Center");
    QVBoxLayout *groupLayout2 = new QVBoxLayout(groupBox2);
    QPushButton *button7 = new QPushButton("Documention One >");
    QPushButton *button8 = new QPushButton("Documention Two >");
    QPushButton *button9 = new QPushButton("Documention Three >");
    button7->setEnabled(false);
    button8->setEnabled(false);
    button9->setEnabled(false);
    groupLayout2->addWidget(button7);
    groupLayout2->addWidget(button8);
    groupLayout2->addWidget(button9);
    layout->addWidget(groupBox2);

}

Form::~Form()
{

}

void Form::paintEvent(QPaintEvent *event)
{
    QPainter painer(this);
    painer.setPen(Qt::NoPen);
    painer.setBrush(Qt::white);
    painer.drawRect(rect());
}
