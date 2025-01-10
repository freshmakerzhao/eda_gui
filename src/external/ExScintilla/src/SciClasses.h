// The definition of various Qt version independent classes used by the rest of
// the port.



#ifndef _SCICLASSES_H
#define _SCICLASSES_H

#include <QListWidget>
#include <QMenu>
#include <QSignalMapper>
#include <QWidget>

#include <Exsci/exsciglobal.h>


class ExsciScintillaQt;
class ExsciListBoxQt;


// A simple QWidget sub-class to implement a call tip.  This is not put into
// the Scintilla namespace because of moc's problems with preprocessor macros.
class ExsciSciCallTip : public QWidget
{
    Q_OBJECT

public:
    ExsciSciCallTip(QWidget *parent, ExsciScintillaQt *sci_);
    ~ExsciSciCallTip();

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);

private:
    ExsciScintillaQt *sci;
};


// A popup menu where options correspond to a numeric command.  This is not put
// into the Scintilla namespace because of moc's problems with preprocessor
// macros.
class ExsciSciPopup : public QMenu
{
    Q_OBJECT

public:
    ExsciSciPopup();

    void addItem(const QString &label, int cmd, bool enabled,
            ExsciScintillaQt *sci_);

private slots:
    void on_triggered(int cmd);

private:
    ExsciScintillaQt *sci;
    QSignalMapper mapper;
};


// This sub-class of QListBox is needed to provide slots from which we can call
// ExsciListBox's double-click callback (and you thought this was a C++
// program).  This is not put into the Scintilla namespace because of moc's
// problems with preprocessor macros.
class ExsciSciListBox : public QListWidget
{
    Q_OBJECT

public:
    ExsciSciListBox(QWidget *parent, ExsciListBoxQt *lbx_);
    virtual ~ExsciSciListBox();

    void addItemPixmap(const QPixmap &pm, const QString &txt);

    int find(const QString &prefix);
    QString text(int n);

protected:
    void keyPressEvent(QKeyEvent *e);

private slots:
    void handleSelection();

private:
    ExsciListBoxQt *lbx;
};

#endif
