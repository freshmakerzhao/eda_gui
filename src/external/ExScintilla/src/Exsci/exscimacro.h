// This defines the interface to the ExsciMacro class.



#ifndef EXSCIMACRO_H
#define EXSCIMACRO_H

#include <QList>
#include <QObject>
#include <QString>

#include <Exsci/exsciglobal.h>


class ExsciScintilla;


//! \brief The ExsciMacro class represents a sequence of recordable editor
//! commands.
//!
//! Methods are provided to convert convert a macro to and from a textual
//! representation so that they can be easily written to and read from
//! permanent storage.
class EXSCINTILLA_EXPORT ExsciMacro : public QObject
{
    Q_OBJECT

public:
    //! Construct a ExsciMacro with parent \a parent.
    ExsciMacro(ExsciScintilla *parent);

    //! Construct a ExsciMacro from the printable ASCII representation \a asc,
    //! with parent \a parent.
    ExsciMacro(const QString &asc, ExsciScintilla *parent);

    //! Destroy the ExsciMacro instance.
    virtual ~ExsciMacro();

    //! Clear the contents of the macro.
    void clear();

    //! Load the macro from the printable ASCII representation \a asc.  Returns
    //! true if there was no error.
    //!
    //! \sa save()
    bool load(const QString &asc);

    //! Return a printable ASCII representation of the macro.  It is guaranteed
    //! that only printable ASCII characters are used and that double quote
    //! characters will not be used.
    //!
    //! \sa load()
    QString save() const;

public slots:
    //! Play the macro.
    virtual void play();

    //! Start recording user commands and add them to the macro.
    virtual void startRecording();

    //! Stop recording user commands.
    virtual void endRecording();

private slots:
    void record(unsigned int msg, unsigned long wParam, void *lParam);

private:
    struct Macro {
        unsigned int msg;
        unsigned long wParam;
        QByteArray text;
    };

    ExsciScintilla *exsci;
    QList<Macro> macro;

    ExsciMacro(const ExsciMacro &);
    ExsciMacro &operator=(const ExsciMacro &);
};

#endif
