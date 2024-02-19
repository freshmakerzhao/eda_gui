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
#include <QListWidget>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QDebug>

class ProjectWizard : public QWizard
{
    Q_OBJECT
public:
    ProjectWizard(QWidget *parent = nullptr);

private slots:
    void onFinish();

signals:
    void wizardAccepted(const QString &path);

public:
    QStringList sourcesFilesList;
    QStringList constrainsFilesList;
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

#endif // PROJECTWIZARD_H
