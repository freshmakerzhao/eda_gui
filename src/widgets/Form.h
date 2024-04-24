#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QPainter>
#include <QDebug>

class Form : public QWidget
{
    Q_OBJECT

public:
    static Form *instance();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Form(QWidget *parent = nullptr);
    ~Form();
};

#endif // FORM_H
