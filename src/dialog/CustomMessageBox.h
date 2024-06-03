#ifndef CUSTOMMESSAGEBOX_H
#define CUSTOMMESSAGEBOX_H

#include <QMessageBox>
#include <QPushButton>

class CustomMessageBox : public QMessageBox
{
public:
    CustomMessageBox(QWidget *parent = nullptr);

    static StandardButton showWarning(QWidget *parent, const QString &title,
                                          const QString &text, StandardButtons buttons = Ok,
                                          StandardButton defaultButton = NoButton);

    static StandardButton showInformation(QWidget *parent, const QString &title,
                                          const QString &text, StandardButtons buttons = Ok,
                                          StandardButton defaultButton = NoButton);

    static StandardButton showQuestion(
            QWidget *parent,
            const QString &title,
            const QString &text,
            StandardButtons buttons = Ok,
            StandardButton defaultButton = NoButton
    );

    static StandardButton showSuccess(QWidget *parent, const QString &title,
                                      const QString &text, StandardButtons buttons = Ok,
                                      StandardButton defaultButton = NoButton);

    static StandardButton showError(QWidget *parent, const QString &title,
                                    const QString &text, StandardButtons buttons = Ok,
                                    StandardButton defaultButton = NoButton);


protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    static void setButtonStyleSheet(CustomMessageBox &messageBox);
};

#endif // CUSTOMMESSAGEBOX_H
