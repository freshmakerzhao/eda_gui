// This defines the specialisation of QListBox that handles the Scintilla
// double-click callback.
 


#include <qmap.h>
#include <qpixmap.h>
#include <qstring.h>

#include "Platform.h"


class XsciSciListBox;


// This is an internal class but it is referenced by a public class so it has
// to have a Xsci prefix rather than being put in the Scintilla namespace.
// However the reason for avoiding this no longer applies.
class XsciListBoxQt : public Scintilla::ListBox
{
public:
    XsciListBoxQt();

    virtual void SetFont(Scintilla::Font &font);
    virtual void Create(Scintilla::Window &parent, int, Scintilla::Point, int,
            bool unicodeMode, int);
    virtual void SetAverageCharWidth(int);
    virtual void SetVisibleRows(int);
    virtual int GetVisibleRows() const;
    virtual Scintilla::PRectangle GetDesiredRect();
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
    virtual void SetDelegate(Scintilla::IListBoxDelegate *lbDelegate);
    virtual void SetList(const char *list, char separator, char typesep);

    void handleDoubleClick();
    void handleRelease();

private:
    XsciSciListBox *slb;
    int visible_rows;
    bool utf8;
    Scintilla::IListBoxDelegate *delegate;

    typedef QMap<int, QPixmap> xpmMap;
    xpmMap xset;

    void selectionChanged();
};
