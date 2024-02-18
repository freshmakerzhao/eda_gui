#ifndef PROJECTWIZARD_H
#define PROJECTWIZARD_H

#include <QWizard>
#include <QLineEdit>
#include <QDebug>
#include <QLabel>

namespace Ui {
class ProjectWizard;
}

class ProjectWizard : public QWizard
{
    Q_OBJECT

public:
    explicit ProjectWizard(QWidget *parent = nullptr);
    ~ProjectWizard();

    enum {Page_Init, Page_AddSources, Page_AddConstraints, Page_Summary};

    void accept() override;

    QString projectName;
    QString projectPath;

public slots:
signals:
    void wizardAccepted(const QString &path);

private:
    Ui::ProjectWizard *ui;
};

class InitPage : public QWizardPage {
    Q_OBJECT

public:
    InitPage(QWidget *parent = 0);

private:

};

class AddSourcesPage : public QWizardPage {
    Q_OBJECT

public:
    AddSourcesPage(QWidget *parent = 0);

private:

};

class AddConstraintsPage : public QWizardPage {
    Q_OBJECT

public:
    AddConstraintsPage(QWidget *parent = 0);

private:
};


class SummaryPage : public QWizardPage {
    Q_OBJECT

public:
    SummaryPage(QWidget *parent = 0);

    void initializePage() override;

private:
    QLabel *namelabel;
    QLabel *prjnamelabel;
    QLabel *pathlabel;
    QLabel *prjpathlabel;
};
#endif // PROJECTWIZARD_H
