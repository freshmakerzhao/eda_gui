/**
  ******************************************************************************
  * @file           : WizTableView.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/10/7
  ******************************************************************************
  */
#ifndef WIZCOMMON_H
#define WIZCOMMON_H

#include <QTableView>
#include <QPainter>

class WizTableView : public QTableView {
public:
    WizTableView(QWidget *parent = nullptr) : QTableView(parent), displayText("No data available") {}

    void setDisplayText(const QString &text) {
        displayText = text;
        viewport()->update();  // 触发重绘
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QTableView::paintEvent(event);

        if (model() == nullptr || model()->rowCount() == 0) {
            QPainter painter(viewport());
            painter.setPen(Qt::gray);
            painter.drawText(viewport()->rect(), Qt::AlignCenter, displayText);
        }
    }

private:
    QString displayText;
};
#endif // WIZCOMMON_H
