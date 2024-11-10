// This module implements the XsciCommand class.
 


#include "Xsci/xscicommand.h"

#include <qnamespace.h>
#include <qapplication.h>

#include "Xsci/xsciscintilla.h"
#include "Xsci/xsciscintillabase.h"


static int convert(int key);


// The ctor.
XsciCommand::XsciCommand(XsciScintilla *qs, XsciCommand::Command cmd, int key,
        int altkey, const char *desc)
    : qsCmd(qs), scicmd(cmd), qkey(key), qaltkey(altkey), descCmd(desc)
{
    scikey = convert(qkey);

    if (scikey)
        qsCmd->SendScintilla(XsciScintillaBase::SCI_ASSIGNCMDKEY, scikey,
                scicmd);

    scialtkey = convert(qaltkey);

    if (scialtkey)
        qsCmd->SendScintilla(XsciScintillaBase::SCI_ASSIGNCMDKEY, scialtkey,
                scicmd);
}


// Execute the command.
void XsciCommand::execute()
{
    qsCmd->SendScintilla(scicmd);
}


// Bind a key to a command.
void XsciCommand::setKey(int key)
{
    bindKey(key,qkey,scikey);
}


// Bind an alternate key to a command.
void XsciCommand::setAlternateKey(int altkey)
{
    bindKey(altkey,qaltkey,scialtkey);
}


// Do the hard work of binding a key.
void XsciCommand::bindKey(int key,int &qk,int &scik)
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
        qsCmd->SendScintilla(XsciScintillaBase::SCI_CLEARCMDKEY, scik);

    qk = key;
    scik = new_scikey;

    if (scik)
        qsCmd->SendScintilla(XsciScintillaBase::SCI_ASSIGNCMDKEY, scik, scicmd);
}


// See if a key is valid.
bool XsciCommand::validKey(int key)
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
        sci_mod |= XsciScintillaBase::SCMOD_SHIFT;

    if (key & Qt::CTRL)
        sci_mod |= XsciScintillaBase::SCMOD_CTRL;

    if (key & Qt::ALT)
        sci_mod |= XsciScintillaBase::SCMOD_ALT;

    if (key & Qt::META)
        sci_mod |= XsciScintillaBase::SCMOD_META;

    key &= ~Qt::MODIFIER_MASK;

    // Convert the key.
    int sci_key = XsciScintillaBase::commandKey(key, sci_mod);

    if (sci_key)
        sci_key |= (sci_mod << 16);

    return sci_key;
}


// Return the translated user friendly description.
QString XsciCommand::description() const
{
    return qApp->translate("XsciCommand", descCmd);
}
