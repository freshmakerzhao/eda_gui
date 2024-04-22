#ifndef CUSTOMMESSAGEBOX_H
#define CUSTOMMESSAGEBOX_H

#include <QMessageBox>
#include <QSpacerItem>
#include <QGridLayout>

class CustomMessageBox : public QMessageBox
{
public:
    CustomMessageBox(QWidget *parent = nullptr);

    static StandardButton showInformation(QWidget *parent, const QString &title,
                                          const QString &text, StandardButtons buttons = Ok,
                                          StandardButton defaultButton = NoButton);

    static StandardButton showQuestion(QWidget *parent, const QString &title,
                                       const QString &text, StandardButtons buttons = Ok,
                                       StandardButton defaultButton = NoButton);

    static StandardButton showSuccess(QWidget *parent, const QString &title,
                                      const QString &text, StandardButtons buttons = Ok,
                                      StandardButton defaultButton = NoButton);

    static StandardButton showError(QWidget *parent, const QString &title,
                                    const QString &text, StandardButtons buttons = Ok,
                                    StandardButton defaultButton = NoButton);


    void resizeEvent(QResizeEvent *event) override;
};

#endif // CUSTOMMESSAGEBOX_H
