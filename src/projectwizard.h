#ifndef PROJECTWIZARD_H
#define PROJECTWIZARD_H

#include <QWizard>
#include <QWizardPage>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QListWidget>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QTimer>
#include <QDebug>

class ProjectWizard : public QWizard
{
    Q_OBJECT
public:
    ProjectWizard(QWidget *parent = nullptr);
    ~ProjectWizard();

private slots:
    void onFinish();

signals:
    void wizardAccepted(const QString &path);

public:
    QStringList sourcesFilesList;
    QStringList constrainsFilesList;
    QString device;
    QString package;
};

class ProjectNamePage : public QWizardPage
{
    Q_OBJECT
public:
    ProjectNamePage(QWidget *parent = nullptr);
};

class AddSourcesPage : public QWizardPage
{
    Q_OBJECT
public:
    AddSourcesPage(QWidget *parent = nullptr);

private:
    QListWidget *filesListWidget;

private slots:
    void onAddFiles();
    void onRemoveFiles();
    void updateFilesList(const QStringList &files);

signals:
    void filesListUpdated(const QStringList &files);
};

class AddConstrainsPage : public QWizardPage
{
    Q_OBJECT
public:
    AddConstrainsPage(QWidget *parent = nullptr);

private:
    QListWidget *filesListWidget;

private slots:
    void onAddFiles();
    void onRemoveFiles();
    void updateFilesList(const QStringList &files);

signals:
    void filesListUpdated(const QStringList &files);
};

class DefaultPartPage :  public QWizardPage
{
    Q_OBJECT
public:
    DefaultPartPage(QWidget *parent = nullptr);

private:
    QTableView *tableView;

private slots:
    void selectPart(const QModelIndex &index);

};

#endif // PROJECTWIZARD_H
