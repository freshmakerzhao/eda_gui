/**
  ******************************************************************************
  * @file           : CustomListItem.h
  * @author         : zs
  * @description    : None
  * @attention      : None
  * @date           : 2024/5/8
  ******************************************************************************
  */
#ifndef HYBRDLINK_CUSTOMLISTWIDGET_H
#define HYBRDLINK_CUSTOMLISTWIDGET_H


#include <QWidget>
#include <QVBoxLayout>
#include <QEvent>
#include <QHoverEvent>
#include <QApplication>
#include <QLabel>

class CustomListWidget : public QWidget {
Q_OBJECT
public:
    CustomListWidget(const QString &projectName, const QString &projectPath, QWidget *parent = nullptr);
    QString projectName;
    QString projectPath;

protected:
protected:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    bool eventFilter(QObject *obj, QEvent *event) override;
//    void leaveEvent(QEvent *event) override;
//    void enterEvent(QEvent *event) override;

signals:
    void getProjectPath(const QString &projectPath);

private:
    QVBoxLayout *layout;
    QLabel *titleLabel;
    QLabel *pathLabel;
};

#endif //HYBRDLINK_CUSTOMLISTWIDGET_H
