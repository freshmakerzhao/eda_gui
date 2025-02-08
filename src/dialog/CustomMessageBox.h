#ifndef CUSTOMMESSAGEBOX_H
#define CUSTOMMESSAGEBOX_H

#include <QMessageBox>
#include <QPushButton>
#include <QApplication>

class CustomMessageBox : public QMessageBox
{
    Q_OBJECT
public:
    CustomMessageBox(QWidget *parent = nullptr);    

    static StandardButton information(QWidget *parent, const QString &title,
                                      const QString &text, StandardButtons buttons = Ok,
                                      StandardButton defaultButton = NoButton);
    static StandardButton question(QWidget *parent, const QString &title,
                                   const QString &text, StandardButtons buttons = StandardButtons(Yes | No),
                                   StandardButton defaultButton = NoButton);
    static StandardButton warning(QWidget *parent, const QString &title,
                                  const QString &text, StandardButtons buttons = Ok,
                                  StandardButton defaultButton = NoButton);
    static StandardButton critical(QWidget *parent, const QString &title,
                                   const QString &text, StandardButtons buttons = Ok,
                                   StandardButton defaultButton = NoButton);

private:
    static StandardButton showCustomMessage(QWidget *parent, const QString &title, const QString &text,
                                            Icon icon, StandardButtons buttons, StandardButton defaultButton);

    static void setButtonStyleSheet(CustomMessageBox &messageBox);

    static const QString HIGHLIGHT_BUTTON_STYLESHEET;

    static const QString DEFAULT_BUTTON_STYLESHEET;
};

#endif // CUSTOMMESSAGEBOX_H
