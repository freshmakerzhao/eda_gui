// This module defines interface to the XsciAbstractAPIs class.
 


#ifndef XSCIABSTRACTAPIS_H
#define XSCIABSTRACTAPIS_H

#include <QList>
#include <QObject>
#include <QStringList>

#include <Xsci/xsciglobal.h>
#include <Xsci/xsciscintilla.h>


class XsciLexer;


//! \brief The XsciAbstractAPIs class represents the interface to the textual
//! API information used in call tips and for auto-completion.  A sub-class
//! will provide the actual implementation of the interface.
//!
//! API information is specific to a particular language lexer but can be
//! shared by multiple instances of the lexer.
class XSCINTILLA_EXPORT XsciAbstractAPIs : public QObject
{
    Q_OBJECT

public:
    //! Constructs a XsciAbstractAPIs instance attached to lexer \a lexer.  \a
    //! lexer becomes the instance's parent object although the instance can
    //! also be subsequently attached to other lexers.
    XsciAbstractAPIs(XsciLexer *lexer);

    //! Destroy the XsciAbstractAPIs instance.
    virtual ~XsciAbstractAPIs();

    //! Return the lexer that the instance is attached to.
    XsciLexer *lexer() const;

    //! Update the list \a list with API entries derived from \a context.  \a
    //! context is the list of words in the text preceding the cursor position.
    //! The characters that make up a word and the characters that separate
    //! words are defined by the lexer.  The last word is a partial word and
    //! may be empty if the user has just entered a word separator.
    virtual void updateAutoCompletionList(const QStringList &context,
            QStringList &list) = 0;

    //! This is called when the user selects the entry \a selection from the
    //! auto-completion list.  A sub-class can use this as a hint to provide
    //! more specific API entries in future calls to
    //! updateAutoCompletionList().  The default implementation does nothing.
    virtual void autoCompletionSelected(const QString &selection);

    //! Return the call tips valid for the context \a context.  (Note that the
    //! last word of the context will always be empty.)  \a commas is the number
    //! of commas the user has typed after the context and before the cursor
    //! position.  The exact position of the list of call tips can be adjusted
    //! by specifying a corresponding left character shift in \a shifts.  This
    //! is normally done to correct for any displayed context according to \a
    //! style.
    //!
    //! \sa updateAutoCompletionList()
    virtual QStringList callTips(const QStringList &context, int commas,
            XsciScintilla::CallTipsStyle style, QList<int> &shifts) = 0;

private:
    XsciLexer *lex;

    XsciAbstractAPIs(const XsciAbstractAPIs &);
    XsciAbstractAPIs &operator=(const XsciAbstractAPIs &);
};

#endif
