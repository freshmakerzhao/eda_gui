#ifndef CUSTOMMESSAGEBOX_H
#define CUSTOMMESSAGEBOX_H

#include <QtWidgets>

class CustomMessageBox : public QMessageBox
{
public:
    CustomMessageBox(QWidget *parent = nullptr);

    // 设置标题和消息
    void setInfo(const QString& title, const QString& message);
    void setWarning(const QString& title, const QString& message);
    void setError(const QString& title, const QString& message);

private:

};

#endif // CUSTOMMESSAGEBOX_H
