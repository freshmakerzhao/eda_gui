// This module implements the ExsciCommand class.



#include "Exsci/exscicommand.h"

#include <qnamespace.h>
#include <qapplication.h>

#include "Exsci/exsciscintilla.h"
#include "Exsci/exsciscintillabase.h"


static int convert(int key);


// The ctor.
ExsciCommand::ExsciCommand(ExsciScintilla *qs, ExsciCommand::Command cmd, int key,
        int altkey, const char *desc)
    : qsCmd(qs), scicmd(cmd), qkey(key), qaltkey(altkey), descCmd(desc)
{
    scikey = convert(qkey);

    if (scikey)
        qsCmd->SendScintilla(ExsciScintillaBase::SCI_ASSIGNCMDKEY, scikey,
                scicmd);

    scialtkey = convert(qaltkey);

    if (scialtkey)
        qsCmd->SendScintilla(ExsciScintillaBase::SCI_ASSIGNCMDKEY, scialtkey,
                scicmd);
}


// Execute the command.
void ExsciCommand::execute()
{
    qsCmd->SendScintilla(scicmd);
}


// Bind a key to a command.
void ExsciCommand::setKey(int key)
{
    bindKey(key,qkey,scikey);
}


// Bind an alternate key to a command.
void ExsciCommand::setAlternateKey(int altkey)
{
    bindKey(altkey,qaltkey,scialtkey);
}


// Do the hard work of binding a key.
void ExsciCommand::bindKey(int key,int &qk,int &scik)
{
    int new_scikey;

    // Ignore if it is invalid, allowing for the fact that we might be
    // unbinding it.
    if (key)
    {
        new_scikey = convert(key);

        if (!new_scikey)
            return;
    }
    else
        new_scikey = 0;

    if (scik)
        qsCmd->SendScintilla(ExsciScintillaBase::SCI_CLEARCMDKEY, scik);

    qk = key;
    scik = new_scikey;

    if (scik)
        qsCmd->SendScintilla(ExsciScintillaBase::SCI_ASSIGNCMDKEY, scik, scicmd);
}


// See if a key is valid.
bool ExsciCommand::validKey(int key)
{
    return convert(key);
}


// Convert a Qt character to the Scintilla equivalent.  Return zero if it is
// invalid.
static int convert(int key)
{
    // Convert the modifiers.
    int sci_mod = 0;

    if (key & Qt::SHIFT)
        sci_mod |= ExsciScintillaBase::SCMOD_SHIFT;

    if (key & Qt::CTRL)
        sci_mod |= ExsciScintillaBase::SCMOD_CTRL;

    if (key & Qt::ALT)
        sci_mod |= ExsciScintillaBase::SCMOD_ALT;

    if (key & Qt::META)
        sci_mod |= ExsciScintillaBase::SCMOD_META;

    key &= ~Qt::MODIFIER_MASK;

    // Convert the key.
    int sci_key = ExsciScintillaBase::commandKey(key, sci_mod);

    if (sci_key)
        sci_key |= (sci_mod << 16);

    return sci_key;
}


// Return the translated user friendly description.
QString ExsciCommand::description() const
{
    return qApp->translate("ExsciCommand", descCmd);
}
