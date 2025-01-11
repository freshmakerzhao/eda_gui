// This defines the specialisation of QListBox that handles the Scintilla
// double-click callback.



#include <qmap.h>
#include <qpixmap.h>
#include <qstring.h>

#include "SciNamespace.h"

#include "Platform.h"


class ExsciSciListBox;


// This is an internal class but it is referenced by a public class so it has
// to have a Exsci prefix rather than being put in the Scintilla namespace
// which would mean exposing the SCI_NAMESPACE mechanism).
class ExsciListBoxQt : public EXSCI_SCI_NAMESPACE(ListBox)
{
public:
    ExsciListBoxQt();

    EXSCI_SCI_NAMESPACE(CallBackAction) cb_action;
    void *cb_data;

    virtual void SetFont(EXSCI_SCI_NAMESPACE(Font) &font);
    virtual void Create(EXSCI_SCI_NAMESPACE(Window) &parent, int,
            EXSCI_SCI_NAMESPACE(Point), int, bool unicodeMode, int);
    virtual void SetAverageCharWidth(int);
    virtual void SetVisibleRows(int);
    virtual int GetVisibleRows() const;
    virtual EXSCI_SCI_NAMESPACE(PRectangle) GetDesiredRect();
    virtual int CaretFromEdge();
    virtual void Clear();
    virtual void Append(char *s, int type = -1);
    virtual int Length();
    virtual void Select(int n);
    virtual int GetSelection();
    virtual int Find(const char *prefix);
    virtual void GetValue(int n, char *value, int len);
    virtual void Sort();
    virtual void RegisterImage(int type, const char *xpm_data);
    virtual void RegisterRGBAImage(int type, int width, int height,
            const unsigned char *pixelsImage);
    virtual void ClearRegisteredImages();
    virtual void SetDoubleClickAction(
            EXSCI_SCI_NAMESPACE(CallBackAction) action, void *data);
    virtual void SetList(const char *list, char separator, char typesep);

private:
    ExsciSciListBox *slb;
    int visible_rows;
    bool utf8;

    typedef QMap<int, QPixmap> xpmMap;
    xpmMap xset;
};
