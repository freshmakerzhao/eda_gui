/**
  ******************************************************************************
  * @file           : RecentItemWidget.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/10/21
  ******************************************************************************
  */
#ifndef RECENTITEMWIDGET_H
#define RECENTITEMWIDGET_H


#include <QWidget>
#include <QVBoxLayout>
#include <QEvent>
#include <QHoverEvent>
#include <QApplication>
#include <QLabel>

class RecentItemWidget : public QWidget {
Q_OBJECT
public:
    RecentItemWidget(const QString &projectName, const QString &projectPath, QWidget *parent = nullptr);
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

#endif // RECENTITEMWIDGET_H
