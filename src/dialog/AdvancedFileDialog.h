#ifndef HBDFILEDIALOG_H
#define HBDFILEDIALOG_H

#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QLabel>
#include <QFileInfo>
#include <QLocale>
#include <QScrollArea>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QStackedWidget>
#include <QSplitter>
#include <QToolBar>
#include <QListView>
#include <QDebug>

class AdvancedFileDialog : public QFileDialog {
    Q_OBJECT

public:
    AdvancedFileDialog(QWidget *parent = nullptr);

    //! 设置当前工作路径
    void setCurWorkDir(const QString &dir);


private slots:
    void updateMetadata(const QString &path);

protected:

private:
    const QString promptText = "File metadata will be shown here";

    //! 当前工作路径
    QString curWorkDir;

    QString formatDateTime(const QDateTime &dateTime);

    QLabel *metadataLabel;

    QTextEdit *metadataEdit;

    QMap<QString, QString> suffixToDescription{
        {"json", "JSON"},
        {"bit", "Bistream File"}
    };

    QToolBar *toolBar;

    QAction *jumpToHomeDirAction;

    QAction *jumpToCurWorkDirAction;

    QAction *jumpToRecPrjDirAction;
};


#endif // HBDFILEDIALOG_H
