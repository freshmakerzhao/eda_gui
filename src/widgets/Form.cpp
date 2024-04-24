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

void Form::paintEvent(QPaintEvent *event)
{
    QPainter painer(this);
    painer.setPen(Qt::NoPen);
    painer.setBrush(Qt::white);
    painer.drawRect(rect());
}


Form::Form(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("QWidget { background-image: url(:/resource/white.png); }"
                  "QGroupBox { font-family: Console; font-size: 30px; }"
                  "QPushButton { border: none; text-align: left; font-size: 22px; }"
                  "QPushButton:hover { color: #4f7cce; }"
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
    QPushButton *createProjectBtn = new QPushButton("Create Project >");
    connect(createProjectBtn, &QPushButton::clicked, MainWindow::instance(), &MainWindow::onNewTriggered);
    QPushButton *openProjectBtn = new QPushButton("Open Project >");
    connect(openProjectBtn, &QPushButton::clicked, MainWindow::instance(), &MainWindow::onOpenTriggered);
    QPushButton *openExampleBtn = new QPushButton("Open Example Project >");
    openExampleBtn->setEnabled(false);
    groupLayout->addWidget(createProjectBtn);
    groupLayout->addWidget(openProjectBtn);
    groupLayout->addWidget(openExampleBtn);
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
    qDebug() << "[Form] Distructing...";
}
