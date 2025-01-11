// This module defines interface to the ExsciAbstractAPIs class.



#ifndef EXSCIABSTRACTAPIS_H
#define EXSCIABSTRACTAPIS_H

#include <QList>
#include <QObject>
#include <QStringList>

#include <Exsci/exsciglobal.h>
#include <Exsci/exsciscintilla.h>


class ExsciLexer;


//! \brief The ExsciAbstractAPIs class represents the interface to the textual
//! API information used in call tips and for auto-completion.  A sub-class
//! will provide the actual implementation of the interface.
//!
//! API information is specific to a particular language lexer but can be
//! shared by multiple instances of the lexer.
class EXSCINTILLA_EXPORT ExsciAbstractAPIs : public QObject
{
    Q_OBJECT

public:
    //! Constructs a ExsciAbstractAPIs instance attached to lexer \a lexer.  \a
    //! lexer becomes the instance's parent object although the instance can
    //! also be subsequently attached to other lexers.
    ExsciAbstractAPIs(ExsciLexer *lexer);

    //! Destroy the ExsciAbstractAPIs instance.
    virtual ~ExsciAbstractAPIs();

    //! Return the lexer that the instance is attached to.
    ExsciLexer *lexer() const;

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
            ExsciScintilla::CallTipsStyle style, QList<int> &shifts) = 0;

private:
    ExsciLexer *lex;

    ExsciAbstractAPIs(const ExsciAbstractAPIs &);
    ExsciAbstractAPIs &operator=(const ExsciAbstractAPIs &);
};

#endif
