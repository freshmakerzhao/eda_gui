#ifndef PROGRAMDEVICEDIALOG_H
#define PROGRAMDEVICEDIALOG_H

#include <QtWidgets>

class ProgramDeviceDialog : public QDialog
{
    Q_OBJECT
public:
    ProgramDeviceDialog(QWidget *parent = nullptr);

protected:
    void accept() override;

private:
    QLineEdit *bitStreamFilePathLineEdit;

    QPushButton *bitStreamFilePathBrowseButton;

    // QLineEdit *debugProbesFilePathLineEdit;

    // QPushButton *debugProbesFilePathBrowseButton;

    void onbitStreamFilePathBrowseButtonClicked();

    // void ondebugProbesFilePathBrowseButtonClicked();
};

#endif // PROGRAMDEVICEDIALOG_H
