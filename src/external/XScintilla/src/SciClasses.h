// The definition of various Qt version independent classes used by the rest of
// the port.
 


#ifndef _SCICLASSES_H
#define _SCICLASSES_H

#include <QListWidget>
#include <QMenu>
#include <QSignalMapper>
#include <QWidget>

#include <Xsci/xsciglobal.h>


class XsciScintillaQt;
class XsciListBoxQt;


// A simple QWidget sub-class to implement a call tip.  This is not put into
// the Scintilla namespace because of moc's problems with preprocessor macros.
class XsciSciCallTip : public QWidget
{
    Q_OBJECT

public:
    XsciSciCallTip(QWidget *parent, XsciScintillaQt *sci_);
    ~XsciSciCallTip();

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);

private:
    XsciScintillaQt *sci;
};


// A popup menu where options correspond to a numeric command.  This is not put
// into the Scintilla namespace because of moc's problems with preprocessor
// macros.
class XsciSciPopup : public QMenu
{
    Q_OBJECT

public:
    XsciSciPopup();

    void addItem(const QString &label, int cmd, bool enabled,
            XsciScintillaQt *sci_);

private slots:
    void on_triggered(int cmd);

private:
    XsciScintillaQt *sci;
    QSignalMapper mapper;
};


// This sub-class of QListBox is needed to provide slots from which we can call
// XsciListBox's double-click callback (and you thought this was a C++
// program).  This is not put into the Scintilla namespace because of moc's
// problems with preprocessor macros.
class XsciSciListBox : public QListWidget
{
    Q_OBJECT

public:
    XsciSciListBox(QWidget *parent, XsciListBoxQt *lbx_);
    virtual ~XsciSciListBox();

    void addItemPixmap(const QPixmap &pm, const QString &txt);

    int find(const QString &prefix);
    QString text(int n);

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    XsciListBoxQt *lbx;
};

#endif
