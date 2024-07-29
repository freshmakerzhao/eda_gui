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
#include <QTreeView>
#include <QDebug>

class AdvancedFileDialog : public QFileDialog {
    Q_OBJECT

public:
    AdvancedFileDialog(QWidget *parent = nullptr);

    //! 设置当前工作路径
    void setCurWorkDir(const QString &dir);

    static QString getOpenFileName(QWidget *parent = nullptr,
                                   const QString &caption = QString(),
                                   const QString &dir = QString(),
                                   const QString &filter = QString(),
                                   QString *selectedFilter = nullptr,
                                   Options options = Options());

    static QStringList getOpenFileNames(QWidget *parent = nullptr,
                                        const QString &caption = QString(),
                                        const QString &dir = QString(),
                                        const QString &filter = QString(),
                                        QString *selectedFilter = nullptr,
                                        Options options = Options());

    static QString getSaveFileName(QWidget *parent = nullptr,
                                   const QString &caption = QString(),
                                   const QString &dir = QString(),
                                   const QString &filter = QString(),
                                   QString *selectedFilter = nullptr,
                                   Options options = Options());

    static QString getExistingDirectory(QWidget *parent = nullptr,
                                        const QString &caption = QString(),
                                        const QString &dir = QString(),
                                        Options options = ShowDirsOnly);


private slots:
    void updateMetadata(const QString &path);

protected:

private:
    void setRecPrjDir(const QString &dir);

    const QString promptText = "File metadata will be shown here";

    //! 当前工作路径
    QString curWorkDir;

    //! 最近使用的工程路径
    QString recPrjDir;

    QString formatDateTime(const QDateTime &dateTime);

    QLabel *metadataLabel;

    QTextEdit *metadataEdit;

    QMap<QString, QString> suffixToDescription{
        {"json", "JSON"},
        {"bit", "Bistream File"},
        {"hpr", "HybrdLink Project File"},
        {"v", "Verilog Source Files"}
    };

    QToolBar *toolBar;

    QAction *jumpToHomeDirAction;

    QAction *jumpToCurWorkDirAction;

    QAction *jumpToRecPrjDirAction;
};


#endif // HBDFILEDIALOG_H
