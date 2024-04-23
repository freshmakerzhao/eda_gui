#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QPainter>

class Form : public QWidget
{
    Q_OBJECT

public:
    static Form *instance();
    explicit Form(QWidget *parent = nullptr);
    ~Form();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
};

#endif // FORM_H
