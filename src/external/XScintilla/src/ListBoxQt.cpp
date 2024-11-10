// This module implements the specialisation of QListBox that handles the
// Scintilla double-click callback.
 


#include "ListBoxQt.h"

#include <stdlib.h>

#include "SciClasses.h"
#include "Xsci/xsciscintilla.h"


XsciListBoxQt::XsciListBoxQt()
    : slb(0), visible_rows(5), utf8(false), delegate(0)
{
}


void XsciListBoxQt::SetFont(Scintilla::Font &font)
{
    QFont *f = reinterpret_cast<QFont *>(font.GetID());

    if (f)
        slb->setFont(*f);
}


void XsciListBoxQt::Create(Scintilla::Window &parent, int, Scintilla::Point,
        int, bool unicodeMode, int)
{
    utf8 = unicodeMode;

    // The parent we want is the XsciScintillaBase, not the text area.
    wid = slb = new XsciSciListBox(reinterpret_cast<QWidget *>(parent.GetID())->parentWidget(), this);
}


void XsciListBoxQt::SetAverageCharWidth(int)
{
    // We rely on sizeHint() for the size of the list box rather than make
    // calculations based on the average character width and the number of
    // visible rows.
}


void XsciListBoxQt::SetVisibleRows(int vrows)
{
    // We only pretend to implement this.
    visible_rows = vrows;
}


int XsciListBoxQt::GetVisibleRows() const
{
    return visible_rows;
}


Scintilla::PRectangle XsciListBoxQt::GetDesiredRect()
{
    Scintilla::PRectangle rc(0, 0, 100, 100);

    if (slb)
    {
        QSize sh = slb->sizeHint();

        rc.right = sh.width();
        rc.bottom = sh.height();
    }

    return rc;
}


int XsciListBoxQt::CaretFromEdge()
{
    int dist = 0;

    // Find the width of the biggest image.
    for (xpmMap::const_iterator it = xset.begin(); it != xset.end(); ++it)
    {
        int w = it.value().width();

        if (dist < w)
            dist = w;
    }

    if (slb)
        dist += slb->frameWidth();

    // Fudge factor - adjust if required.
    dist += 3;

    return dist;
}


void XsciListBoxQt::Clear()
{
    Q_ASSERT(slb);

    slb->clear();
}


void XsciListBoxQt::Append(char *s, int type)
{
    Q_ASSERT(slb);

    QString qs;

    if (utf8)
        qs = QString::fromUtf8(s);
    else
        qs = QString::fromLatin1(s);

    xpmMap::const_iterator it;

    if (type < 0 || (it = xset.find(type)) == xset.end())
        slb->addItem(qs);
    else
        slb->addItemPixmap(it.value(), qs);
}


int XsciListBoxQt::Length()
{
    Q_ASSERT(slb);

    return slb->count();
}


void XsciListBoxQt::Select(int n)
{
    Q_ASSERT(slb);

    slb->setCurrentRow(n);
    selectionChanged();
}


int XsciListBoxQt::GetSelection()
{
    Q_ASSERT(slb);

    return slb->currentRow();
}


int XsciListBoxQt::Find(const char *prefix)
{
    Q_ASSERT(slb);

    return slb->find(prefix);
}


void XsciListBoxQt::GetValue(int n, char *value, int len)
{
    Q_ASSERT(slb);

    QString selection = slb->text(n);

    bool trim_selection = false;
    QObject *sci_obj = slb->parent();

    if (sci_obj->inherits("XsciScintilla"))
    {
        XsciScintilla *sci = static_cast<XsciScintilla *>(sci_obj);

        if (sci->isAutoCompletionList())
        {
            // Save the full selection and trim the value we return.
            sci->acSelection = selection;
            trim_selection = true;
        }
    }

    if (selection.isEmpty() || len <= 0)
        value[0] = '\0';
    else
    {
        const char *s;
        int slen;

        QByteArray bytes;

        if (utf8)
            bytes = selection.toUtf8();
        else
            bytes = selection.toLatin1();

        s = bytes.data();
        slen = bytes.length();

        while (slen-- && len--)
        {
            if (trim_selection && *s == ' ')
                break;

            *value++ = *s++;
        }

        *value = '\0';
    }
}


void XsciListBoxQt::Sort()
{
    Q_ASSERT(slb);

    slb->sortItems();
}


void XsciListBoxQt::RegisterImage(int type, const char *xpm_data)
{
    xset.insert(type, *reinterpret_cast<const QPixmap *>(xpm_data));
}


void XsciListBoxQt::RegisterRGBAImage(int type, int, int,
        const unsigned char *pixelsImage)
{
    QPixmap pm;

#if QT_VERSION >= 0x040700
    pm.convertFromImage(*reinterpret_cast<const QImage *>(pixelsImage));
#else
    pm = QPixmap::fromImage(*reinterpret_cast<const QImage *>(pixelsImage));
#endif

    xset.insert(type, pm);
}


void XsciListBoxQt::ClearRegisteredImages()
{
    xset.clear();
}


void XsciListBoxQt::SetDelegate(Scintilla::IListBoxDelegate *lbDelegate)
{
    delegate = lbDelegate;
}


void XsciListBoxQt::handleDoubleClick()
{
    if (delegate)
    {
        Scintilla::ListBoxEvent event(
                Scintilla::ListBoxEvent::EventType::doubleClick);

        delegate->ListNotify(&event);
    }
}


void XsciListBoxQt::handleRelease()
{
    selectionChanged();
}


void XsciListBoxQt::selectionChanged()
{
    if (delegate)
    {
        Scintilla::ListBoxEvent event(
                Scintilla::ListBoxEvent::EventType::selectionChange);

        delegate->ListNotify(&event);
    }
}


void XsciListBoxQt::SetList(const char *list, char separator, char typesep)
{
    char *words;

    Clear();

    if ((words = qstrdup(list)) != NULL)
    {
        char *startword = words;
        char *numword = NULL;

        for (int i = 0; words[i] != '\0'; i++)
        {
            if (words[i] == separator)
            {
                words[i] = '\0';

                if (numword)
                    *numword = '\0';

                Append(startword, numword ? atoi(numword + 1) : -1);

                startword = words + i + 1;
                numword = NULL;
            }
            else if (words[i] == typesep)
            {
                numword = words + i;
            }
        }

        if (startword)
        {
            if (numword)
                *numword = '\0';

            Append(startword, numword ? atoi(numword + 1) : -1);
        }

        delete[] words;
    }
}


// The ListBox methods that need to be implemented explicitly.

Scintilla::ListBox::ListBox() noexcept
{
}


Scintilla::ListBox::~ListBox()
{
}


Scintilla::ListBox *Scintilla::ListBox::Allocate()
{
    return new XsciListBoxQt();
}
