// This defines the interface to the XsciDocument class.
 


#ifndef XSCIDOCUMENT_H
#define XSCIDOCUMENT_H

#include <Xsci/xsciglobal.h>


class XsciScintillaBase;
class XsciDocumentP;


//! \brief The XsciDocument class represents a document to be edited.
//!
//! It is an opaque class that can be attached to multiple instances of
//! XsciScintilla to create different simultaneous views of the same document.
//! XsciDocument uses implicit sharing so that copying class instances is a
//! cheap operation.
class XSCINTILLA_EXPORT XsciDocument
{
public:
    //! Create a new unattached document.
    XsciDocument();
    virtual ~XsciDocument();

    XsciDocument(const XsciDocument &);
    XsciDocument &operator=(const XsciDocument &);

private:
    friend class XsciScintilla;

    void attach(const XsciDocument &that);
    void detach();
    void display(XsciScintillaBase *qsb, const XsciDocument *from);
    void undisplay(XsciScintillaBase *qsb);

    bool isModified() const;
    void setModified(bool m);

    XsciDocumentP *pdoc;
};

#endif
