/**
  ******************************************************************************
  * @file           : RecentItemWidget.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/10/21
  ******************************************************************************
  */
#include "RecentItemWidget.h"

RecentItemWidget::RecentItemWidget(const QString &projectName, const QString &projectPath, QWidget *parent) : QWidget(parent) {
    layout = new QVBoxLayout(this);
    this->projectName = projectName;
    this->projectPath = projectPath;
    titleLabel = new QLabel(projectName);
    pathLabel = new QLabel(projectPath);

    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(10);
    titleLabel->setFont(titleFont);

    QFont pathFont = pathLabel->font();
    pathFont.setPointSize(9);
    pathLabel->setFont(pathFont);
    pathLabel->setStyleSheet("QLabel { color: rgb(134, 135, 135); }");

    layout->addWidget(titleLabel);
    layout->addWidget(pathLabel);
    layout->setSpacing(0);
    layout->setContentsMargins(5, 5, 5, 5);
    setAutoFillBackground(true);  // 确保背景颜色被填充
    this->setStyleSheet(
            "background-color: transparent;"
    );
    this->setLayout(layout);
    this->installEventFilter(this);
    // 设置大小策略
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

QSize RecentItemWidget::sizeHint() const
{
    int width = titleLabel->sizeHint().width() > pathLabel->sizeHint().width() ? titleLabel->sizeHint().width() : pathLabel->sizeHint().width();
    int height = titleLabel->sizeHint().height() + pathLabel->sizeHint().height();
    return QSize(width + layout->contentsMargins().left() + layout->contentsMargins().right(), height + layout->contentsMargins().top() + layout->contentsMargins().bottom());
}

QSize RecentItemWidget::minimumSizeHint() const
{
    return sizeHint();
}

bool RecentItemWidget::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        emit getProjectPath(this->projectPath);
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

//bool RecentItemWidget::eventFilter(QObject *obj, QEvent *event) {
//    std::cout << 123;
//    if (obj == this) {
//        if (event->type() == QEvent::HoverEnter) {
//            this->setStyleSheet("background-color: lightgray;");
//        } else if (event->type() == QEvent::HoverLeave) {
//            this->setStyleSheet("background-color: white;");
//        }
//    }
//    return QWidget::eventFilter(obj, event);
//}

//void RecentItemWidget::enterEvent(QEvent *event) {
//    std::cout << 123;
//    Q_UNUSED(event);
//    QPalette pal = palette();
//    pal.setColor(QPalette::Window, QColor(255, 0, 0));  // 设置 hover 背景色
//    setPalette(pal);
//}
//
//void RecentItemWidget::leaveEvent(QEvent *event) {
//    std::cout << 456;
//    Q_UNUSED(event);
//    QPalette pal = palette();
//    pal.setColor(QPalette::Window, palette().window().color());  // 还原背景色
//    setPalette(pal);
//}
