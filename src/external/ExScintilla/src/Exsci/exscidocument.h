// This defines the interface to the ExsciDocument class.



#ifndef EXSCIDOCUMENT_H
#define EXSCIDOCUMENT_H

#include <Exsci/exsciglobal.h>


class ExsciScintillaBase;
class ExsciDocumentP;


//! \brief The ExsciDocument class represents a document to be edited.
//!
//! It is an opaque class that can be attached to multiple instances of
//! ExsciScintilla to create different simultaneous views of the same document.
//! ExsciDocument uses implicit sharing so that copying class instances is a
//! cheap operation.
class EXSCINTILLA_EXPORT ExsciDocument
{
public:
    //! Create a new unattached document.
    ExsciDocument();
    virtual ~ExsciDocument();

    ExsciDocument(const ExsciDocument &);
    ExsciDocument &operator=(const ExsciDocument &);

private:
    friend class ExsciScintilla;

    void attach(const ExsciDocument &that);
    void detach();
    void display(ExsciScintillaBase *qsb, const ExsciDocument *from);
    void undisplay(ExsciScintillaBase *qsb);

    bool isModified() const;
    void setModified(bool m);

    ExsciDocumentP *pdoc;
};

#endif
