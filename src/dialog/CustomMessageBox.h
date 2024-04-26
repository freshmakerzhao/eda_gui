#ifndef CUSTOMMESSAGEBOX_H
#define CUSTOMMESSAGEBOX_H

#include <QMessageBox>
#include <QSpacerItem>
#include <QGridLayout>

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

    // 两个选项的询问对话框
    static StandardButton showTwoOptionQuestion(
            QWidget *parent,
            const QString &title,
            const QString &text,
            StandardButton buttonLeft = Ok,
            StandardButton buttonRight = Cancel,
            Icon defaultIcon = Question,
            StandardButton defaultButton = NoButton
    );

    static StandardButton showSuccess(QWidget *parent, const QString &title,
                                      const QString &text, StandardButtons buttons = Ok,
                                      StandardButton defaultButton = NoButton);

    static StandardButton showError(QWidget *parent, const QString &title,
                                    const QString &text, StandardButtons buttons = Ok,
                                    StandardButton defaultButton = NoButton);


    void resizeEvent(QResizeEvent *event) override;
};

#endif // CUSTOMMESSAGEBOX_H
