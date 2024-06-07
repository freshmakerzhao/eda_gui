/**
  ******************************************************************************
  * @file           : BlockMemoryGenerator.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/6/3
  ******************************************************************************
  */
#include "BlockMemoryGenerator.h"

BlockMemoryGenerator::BlockMemoryGenerator(QWidget *parent) :
    QDialog(parent)
{
    resize(1200, 800);
    setWindowTitle("Custom IP");
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    // vLayout->setAlignment(Qt::AlignTop);
    // ! ---------------------- C区 ----------------------------
    // C区基控件
    QWidget *Cwidget = new QWidget(this);
    // C区布局
    QVBoxLayout *CvLayout = new QVBoxLayout(Cwidget);
    CvLayout->setMargin(0);
    QFormLayout *cmptNameLayout = new QFormLayout;
    cmptNameLineEdit = new QLineEdit(this);
    cmptNameLineEdit->setText("blk_mem_gen_0");
    cmptNameLayout->addRow("Component Name", cmptNameLineEdit);
    tabWidget = new QTabWidget(this);
    CvLayout->addLayout(cmptNameLayout);
    CvLayout->addWidget(tabWidget);

    // ! ---------------------- B区 ----------------------------
    viewModule = new QGraphicsView;

    // ! ---------------------- A区 ----------------------------
    QVBoxLayout *AvLayout = new QVBoxLayout;
    QHBoxLayout *titleLayout = new QHBoxLayout;
    // titleLayout->setAlignment(Qt::AlignLeading);
    QLabel *ipNameLabel = new QLabel("Block Memory Generator(8.4)", this);
    ipNameLabel->setStyleSheet("font-size: 20px;"
                                "font-weight: bold;");
    // ipNameLabel->setFixedHeight(30);
    // AvLayout->addWidget(ipNameLabel);
    QLabel *logoLabel = new QLabel(this);
    logoLabel->setAlignment(Qt::AlignRight);
    QPixmap image(":/resource/icon.png");
    titleLayout->addWidget(ipNameLabel);
    titleLayout->addWidget(logoLabel);
    logoLabel->setPixmap(image.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    AvLayout->addLayout(titleLayout);
    AvLayout->setAlignment(Qt::AlignTop);
    QToolBar *toolBar = new QToolBar(this);
    AvLayout->addWidget(toolBar);
    docAction = new QAction("Documentation", this);
    toolBar->addAction(docAction);
    ipLocAction = new QAction("IP Location", this);
    connect(ipLocAction, &QAction::triggered, this, &BlockMemoryGenerator::onipLocActionTrigger);
    toolBar->addAction(ipLocAction);
    swToDftAction = new QAction("Switch to Defaults", this);
    toolBar->addAction(swToDftAction);
    // ! --------------------- 总布局 ---------------------------
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->setHandleWidth(15);
    splitter->addWidget(viewModule);
    splitter->addWidget(Cwidget);
    vLayout->addLayout(AvLayout);
    vLayout->addWidget(splitter);
    vLayout->setStretchFactor(AvLayout, 1);
    vLayout->setStretchFactor(splitter, 13);

    basic = new BasicWidget(this);
    connect(basic, &BasicWidget::memoryTypeComboBoxChangeSignal, this, &BlockMemoryGenerator::updateMemoryType);
    portAOptions = new PortAOptionsWidget(this);
    portBOptions = new PortBOptionsWidget(this);
    otherOptions = new OtherOptionsWidget(this);
    summary = new SummaryWidget(this);

    tabWidget->addTab(basic, "Basic");
    tabWidget->addTab(portAOptions, "Port A Options");
    tabWidget->addTab(otherOptions, "Other Options");
    tabWidget->addTab(summary, "Summary");

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    // QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    // QPushButton *cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, [this](){
        this->accept();
    });

    connect(buttonBox, &QDialogButtonBox::rejected, [this](){
        this->accept();
    });

    vLayout->addWidget(buttonBox);
}

int BlockMemoryGenerator::exec()
{
    QDialog::exec();
    return 0;
}

void BlockMemoryGenerator::updateMemoryType(const QString &option)
{
    qDebug() << "Memory Type : " << option;
    summary->setMemoryTypeInformation(option);
    // ! 当option为Simple Dual Port RAM |
    // ！          True Dual Port RAM  |
    // !           Dual Port ROM
    // ! 显示Port B Options
    if (option == "Simple Dual Port RAM" ||
        option == "True Dual Port RAM" ||
        option == "Dual Port ROM") {
        tabWidget->insertTab(2, portBOptions, "Port B Options");
        return;
    }
    if (tabWidget->count() == 5) {
        tabWidget->removeTab(2);
    }

}

void BlockMemoryGenerator::onipLocActionTrigger()
{
    IPlocationDialog dialog(this);
    dialog.exec();
}
