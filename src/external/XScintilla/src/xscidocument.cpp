// This module implements the XsciDocument class.
 


#include "Xsci/xscidocument.h"
#include "Xsci/xsciscintillabase.h"


// This internal class encapsulates the underlying document and is shared by
// XsciDocument instances.
class XsciDocumentP
{
public:
    XsciDocumentP() : doc(0), nr_displays(0), nr_attaches(1), modified(false) {}

    void *doc;              // The Scintilla document.
    int nr_displays;        // The number of displays.
    int nr_attaches;        // The number of attaches.
    bool modified;          // Set if not at a save point.
};


// The ctor.
XsciDocument::XsciDocument()
{
    pdoc = new XsciDocumentP();
}


// The dtor.
XsciDocument::~XsciDocument()
{
    detach();
}


// The copy ctor.
XsciDocument::XsciDocument(const XsciDocument &that)
{
    attach(that);
}


// The assignment operator.
XsciDocument &XsciDocument::operator=(const XsciDocument &that)
{
    if (pdoc != that.pdoc)
    {
        detach();
        attach(that);
    }

    return *this;
}


// Attach an existing document to this one.
void XsciDocument::attach(const XsciDocument &that)
{
    ++that.pdoc->nr_attaches;
    pdoc = that.pdoc;
}


// Detach the underlying document.
void XsciDocument::detach()
{
    if (!pdoc)
        return;

    if (--pdoc->nr_attaches == 0)
    {
        if (pdoc->doc && pdoc->nr_displays == 0)
        {
            XsciScintillaBase *qsb = XsciScintillaBase::pool();

            // Release the explicit reference to the document.  If the pool is
            // empty then we just accept the memory leak.
            if (qsb)
                qsb->SendScintilla(XsciScintillaBase::SCI_RELEASEDOCUMENT, 0,
                        pdoc->doc);
        }

        delete pdoc;
    }

    pdoc = 0;
}


// Undisplay and detach the underlying document.
void XsciDocument::undisplay(XsciScintillaBase *qsb)
{
    if (--pdoc->nr_attaches == 0)
        delete pdoc;
    else if (--pdoc->nr_displays == 0)
    {
        // Create an explicit reference to the document to keep it alive.
        qsb->SendScintilla(XsciScintillaBase::SCI_ADDREFDOCUMENT, 0, pdoc->doc);
    }

    pdoc = 0;
}


// Display the underlying document.
void XsciDocument::display(XsciScintillaBase *qsb, const XsciDocument *from)
{
    void *ndoc = (from ? from->pdoc->doc : 0);

    // SCI_SETDOCPOINTER appears to reset the EOL mode so save and restore it.
    int eol_mode = qsb->SendScintilla(XsciScintillaBase::SCI_GETEOLMODE);

    qsb->SendScintilla(XsciScintillaBase::SCI_SETDOCPOINTER, 0, ndoc);
    ndoc = qsb->SendScintillaPtrResult(XsciScintillaBase::SCI_GETDOCPOINTER);

    qsb->SendScintilla(XsciScintillaBase::SCI_SETEOLMODE, eol_mode);

    pdoc->doc = ndoc;
    ++pdoc->nr_displays;
}


// Return the modified state of the document.
bool XsciDocument::isModified() const
{
    return pdoc->modified;
}


// Set the modified state of the document.
void XsciDocument::setModified(bool m)
{
    pdoc->modified = m;
}
