#include "BaseDialog.h"

BaseDialog::BaseDialog(QWidget *parent)
{
    resize(1600, 900);
    setWindowTitle("Custom IP");
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    // vLayout->setAlignment(Qt::AlignTop);
    setStyleSheet("QScrollArea{"
                  " border: none;"
                  "}");

    // ! ---------------------- C区 ----------------------------
    // C区基控件
    QWidget *Cwidget = new QWidget(this);
    // C区布局
    QVBoxLayout *CvLayout = new QVBoxLayout(Cwidget);
    CvLayout->setMargin(0);
    QFormLayout *cmptNameLayout = new QFormLayout;
    componentNameLineEdit = new QLineEdit(this);
    componentNameLineEdit->setText("clk_wiz_0");
    cmptNameLayout->addRow("Component Name", componentNameLineEdit);
    tabWidget = new QTabWidget(this);
    CvLayout->addLayout(cmptNameLayout);
    CvLayout->addWidget(tabWidget);

    // ! ---------------------- B区 ----------------------------
    // viewModule = new QGraphicsView;

    // ! ---------------------- A区 ----------------------------
    QVBoxLayout *AvLayout = new QVBoxLayout;
    QHBoxLayout *titleLayout = new QHBoxLayout;
    // titleLayout->setAlignment(Qt::AlignLeading);
    QLabel *ipNameLabel = new QLabel("Clocking Wizard(6.0)", this);
    ipNameLabel->setStyleSheet("font-size: 20px;"
                               "font-weight: bold;");
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
    toolBar->addAction(ipLocAction);
    switchToDefaultAction = new QAction("Switch to Defaults", this);
    toolBar->addAction(switchToDefaultAction);
    // ! --------------------- 总布局 ---------------------------
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->setHandleWidth(15);
    // splitter->addWidget(viewModule);
    splitter->addWidget(Cwidget);
    QList<int> list;
    list << 500 << 1100;
    splitter->setSizes(list);
    vLayout->addLayout(AvLayout);
    vLayout->addWidget(splitter);
    vLayout->setStretchFactor(AvLayout, 1);
    vLayout->setStretchFactor(splitter, 13);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &BaseDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &BaseDialog::reject);

    vLayout->addWidget(buttonBox);
}
