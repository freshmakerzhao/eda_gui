#ifndef WIZARD_H
#define WIZARD_H

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
#include <QSortFilterProxyModel>
#include <QComboBox>
#include <QFormLayout>
#include <QToolBar>
#include <QDebug>

#include "project.h"

class Wizard : public QWizard
{
    Q_OBJECT
public:
    Wizard(QWidget *parent = nullptr);
    ~Wizard();

private slots:
    void onFinish();

public:
    QStringList sourcesFilesList;
    QStringList constraintFilesList;
    QString part;
    QString archName;
    QString arch;

private:
    Project *project;
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
    void onCreateFile();
    void onRemoveFiles();
    void updateFilesList(const QStringList &files);

signals:
    void filesListUpdatedSignal(const QStringList &files);
};

class AddConstraintPage : public QWizardPage
{
    Q_OBJECT
public:
    AddConstraintPage(QWidget *parent = nullptr);

private:
    QListWidget *filesListWidget;

private slots:
    void onAddFiles();
    void onCreateFile();
    void onRemoveFiles();
    void updateFilesList(const QStringList &files);

signals:
    void filesListUpdatedSignal(const QStringList &files);
};

class DefaultPartPage :  public QWizardPage
{
    Q_OBJECT
public:
    DefaultPartPage(QWidget *parent = nullptr);

protected:
    bool isComplete()  const override;

private:
    QTableView *tableView;

private slots:
    void selectPart(const QModelIndex &index);

};

#endif // WIZARD_H
