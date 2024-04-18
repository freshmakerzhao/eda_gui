#include "CustomMessageBox.h"

CustomMessageBox::CustomMessageBox(QWidget *parent)
    :QMessageBox(parent)
{
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
}

void CustomMessageBox::setInfo(const QString& title, const QString& message)
{
    setWindowTitle(title);
    setText(message);
    setIcon(QMessageBox::Information);
}

void CustomMessageBox::setWarning(const QString& title, const QString& message)
{
    setWindowTitle(title);
    setText(message);
    setIcon(QMessageBox::Warning);
}

void CustomMessageBox::setError(const QString& title, const QString& message)
{
    setWindowTitle(title);
    setText(message);
    setIcon(QMessageBox::Critical);
}

