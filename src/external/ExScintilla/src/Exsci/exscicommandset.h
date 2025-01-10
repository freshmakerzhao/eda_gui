// This defines the interface to the ExsciCommandSet class.



#ifndef EXSCICOMMANDSET_H
#define EXSCICOMMANDSET_H

#include <qglobal.h>

#include <QList>

#include <Exsci/exsciglobal.h>
#include <Exsci/exscicommand.h>


QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

class ExsciScintilla;


//! \brief The ExsciCommandSet class represents the set of all internal editor
//! commands that may have keys bound.
//!
//! Methods are provided to access the individual commands and to read and
//! write the current bindings from and to settings files.
class EXSCINTILLA_EXPORT ExsciCommandSet
{
public:
    //! The key bindings for each command in the set are read from the
    //! settings \a qs.  \a prefix is prepended to the key of each entry.
    //! true is returned if there was no error.
    //!
    //! \sa writeSettings()
    bool readSettings(QSettings &qs, const char *prefix = "/Scintilla");

    //! The key bindings for each command in the set are written to the
    //! settings \a qs.  \a prefix is prepended to the key of each entry.
    //! true is returned if there was no error.
    //!
    //! \sa readSettings()
    bool writeSettings(QSettings &qs, const char *prefix = "/Scintilla");

    //! The commands in the set are returned as a list.
    QList<ExsciCommand *> &commands() {return cmds;}

    //! The primary keys bindings for all commands are removed.
    void clearKeys();

    //! The alternate keys bindings for all commands are removed.
    void clearAlternateKeys();

    // Find the command that is bound to \a key.
    ExsciCommand *boundTo(int key) const;

    // Find a specific command \a command.
    ExsciCommand *find(ExsciCommand::Command command) const;

private:
    friend class ExsciScintilla;

    ExsciCommandSet(ExsciScintilla *qs);
    ~ExsciCommandSet();

    ExsciScintilla *exsci;
    QList<ExsciCommand *> cmds;

    ExsciCommandSet(const ExsciCommandSet &);
    ExsciCommandSet &operator=(const ExsciCommandSet &);
};

#endif
