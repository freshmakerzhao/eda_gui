// The implementation of various Qt version independent classes used by the
// rest of the port.



#include "SciNamespace.h"
#include "SciClasses.h"

#include <QCoreApplication>
#include <QKeyEvent>
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>

#include "ScintillaQt.h"
#include "ListBoxQt.h"


// Create a call tip.
ExsciSciCallTip::ExsciSciCallTip(QWidget *parent, ExsciScintillaQt *sci_)
    : QWidget(parent, Qt::WindowFlags(Qt::Popup|Qt::FramelessWindowHint|Qt::WA_StaticContents)),
      sci(sci_)
{
    // Ensure that the main window keeps the focus (and the caret flashing)
    // when this is displayed.
    setFocusProxy(parent);
}


// Destroy a call tip.
ExsciSciCallTip::~ExsciSciCallTip()
{
    // Ensure that the main window doesn't receive a focus out event when
    // this is destroyed.
    setFocusProxy(0);
}


// Paint a call tip.
void ExsciSciCallTip::paintEvent(QPaintEvent *)
{
    EXSCI_SCI_NAMESPACE(Surface) *surfaceWindow = EXSCI_SCI_NAMESPACE(Surface)::Allocate(SC_TECHNOLOGY_DEFAULT);

    if (!surfaceWindow)
        return;

    QPainter p(this);

    surfaceWindow->Init(&p);
    surfaceWindow->SetUnicodeMode(sci->CodePage() == SC_CP_UTF8);
    sci->ct.PaintCT(surfaceWindow);

    delete surfaceWindow;
}


// Handle a mouse press in a call tip.
void ExsciSciCallTip::mousePressEvent(QMouseEvent *e)
{
    EXSCI_SCI_NAMESPACE(Point) pt;

    pt.x = e->x();
    pt.y = e->y();

    sci->ct.MouseClick(pt);
    sci->CallTipClick();

    update();
}


// Create the popup instance.
ExsciSciPopup::ExsciSciPopup()
{
    // Set up the mapper.
    connect(&mapper, SIGNAL(mapped(int)), this, SLOT(on_triggered(int)));
}


// Add an item and associated command to the popup and enable it if required.
void ExsciSciPopup::addItem(const QString &label, int cmd, bool enabled,
        ExsciScintillaQt *sci_)
{
    QAction *act = addAction(label, &mapper, SLOT(map()));
    mapper.setMapping(act, cmd);
    act->setEnabled(enabled);
    sci = sci_;
}


// A slot to handle a menu action being triggered.
void ExsciSciPopup::on_triggered(int cmd)
{
    sci->Command(cmd);
}


ExsciSciListBox::ExsciSciListBox(QWidget *parent, ExsciListBoxQt *lbx_)
    : QListWidget(parent), lbx(lbx_)
{
    setAttribute(Qt::WA_StaticContents);

#if defined(Q_OS_WIN)
    setWindowFlags(Qt::Tool|Qt::FramelessWindowHint);

    // This stops the main widget losing focus when the user clicks on this one
    // (which prevents this one being destroyed).
    setFocusPolicy(Qt::NoFocus);
#else
    // This is the root of the focus problems under Gnome's window manager.  We
    // have tried many flag combinations in the past.  The consensus now seems
    // to be that the following works.  However it might now work because of a
    // change in Qt so we only enable it for recent versions in order to
    // reduce the risk of breaking something that works with earlier versions.
#if QT_VERSION >= 0x040500
    setWindowFlags(Qt::ToolTip|Qt::WindowStaysOnTopHint);
#else
    setWindowFlags(Qt::Tool|Qt::FramelessWindowHint);
#endif

    // This may not be needed.
    setFocusProxy(parent);
#endif

    setFrameShape(StyledPanel);
    setFrameShadow(Plain);

    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
            SLOT(handleSelection()));
}


void ExsciSciListBox::addItemPixmap(const QPixmap &pm, const QString &txt)
{
    new QListWidgetItem(pm, txt, this);
}


int ExsciSciListBox::find(const QString &prefix)
{
    QList<QListWidgetItem *> itms = findItems(prefix,
            Qt::MatchStartsWith|Qt::MatchCaseSensitive);

    if (itms.size() == 0)
        return -1;

    return row(itms[0]);
}


QString ExsciSciListBox::text(int n)
{
    QListWidgetItem *itm = item(n);

    if (!itm)
        return QString();

    return itm->text();
}


// Reimplemented to close the list when the user presses Escape.
void ExsciSciListBox::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
    {
        e->accept();
        close();
    }
    else
    {
        QListWidget::keyPressEvent(e);

        if (!e->isAccepted())
            QCoreApplication::sendEvent(parent(), e);
    }
}


ExsciSciListBox::~ExsciSciListBox()
{
    // Ensure that the main widget doesn't get a focus out event when this is
    // destroyed.
    setFocusProxy(0);
}


void ExsciSciListBox::handleSelection()
{
    if (lbx && lbx->cb_action)
        lbx->cb_action(lbx->cb_data);
}
