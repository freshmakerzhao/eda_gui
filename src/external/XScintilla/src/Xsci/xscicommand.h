// This defines the interface to the XsciCommand class.
 


#ifndef XSCICOMMAND_H
#define XSCICOMMAND_H

#include <qstring.h>

#include <Xsci/xsciglobal.h>
#include <Xsci/xsciscintillabase.h>


class XsciScintilla;


//! \brief The XsciCommand class represents an internal editor command that may
//! have one or two keys bound to it.
//!
//! Methods are provided to change the keys bound to the command and to remove
//! a key binding.  Each command has a user friendly description of the command
//! for use in key mapping dialogs.
class XSCINTILLA_EXPORT XsciCommand
{
public:
    //! This enum defines the different commands that can be assigned to a key.
    enum Command {
        //! Move down one line.
        LineDown = XsciScintillaBase::SCI_LINEDOWN,

        //! Extend the selection down one line.
        LineDownExtend = XsciScintillaBase::SCI_LINEDOWNEXTEND,

        //! Extend the rectangular selection down one line.
        LineDownRectExtend = XsciScintillaBase::SCI_LINEDOWNRECTEXTEND,

        //! Scroll the view down one line.
        LineScrollDown = XsciScintillaBase::SCI_LINESCROLLDOWN,

        //! Move up one line.
        LineUp = XsciScintillaBase::SCI_LINEUP,

        //! Extend the selection up one line.
        LineUpExtend = XsciScintillaBase::SCI_LINEUPEXTEND,

        //! Extend the rectangular selection up one line.
        LineUpRectExtend = XsciScintillaBase::SCI_LINEUPRECTEXTEND,

        //! Scroll the view up one line.
        LineScrollUp = XsciScintillaBase::SCI_LINESCROLLUP,

        //! Scroll to the start of the document.
        ScrollToStart = XsciScintillaBase::SCI_SCROLLTOSTART,

        //! Scroll to the end of the document.
        ScrollToEnd = XsciScintillaBase::SCI_SCROLLTOEND,

        //! Scroll vertically to centre the current line.
        VerticalCentreCaret = XsciScintillaBase::SCI_VERTICALCENTRECARET,

        //! Move down one paragraph.
        ParaDown = XsciScintillaBase::SCI_PARADOWN,

        //! Extend the selection down one paragraph.
        ParaDownExtend = XsciScintillaBase::SCI_PARADOWNEXTEND,

        //! Move up one paragraph.
        ParaUp = XsciScintillaBase::SCI_PARAUP,

        //! Extend the selection up one paragraph.
        ParaUpExtend = XsciScintillaBase::SCI_PARAUPEXTEND,

        //! Move left one character.
        CharLeft = XsciScintillaBase::SCI_CHARLEFT,

        //! Extend the selection left one character.
        CharLeftExtend = XsciScintillaBase::SCI_CHARLEFTEXTEND,

        //! Extend the rectangular selection left one character.
        CharLeftRectExtend = XsciScintillaBase::SCI_CHARLEFTRECTEXTEND,

        //! Move right one character.
        CharRight = XsciScintillaBase::SCI_CHARRIGHT,

        //! Extend the selection right one character.
        CharRightExtend = XsciScintillaBase::SCI_CHARRIGHTEXTEND,

        //! Extend the rectangular selection right one character.
        CharRightRectExtend = XsciScintillaBase::SCI_CHARRIGHTRECTEXTEND,

        //! Move left one word.
        WordLeft = XsciScintillaBase::SCI_WORDLEFT,

        //! Extend the selection left one word.
        WordLeftExtend = XsciScintillaBase::SCI_WORDLEFTEXTEND,

        //! Move right one word.
        WordRight = XsciScintillaBase::SCI_WORDRIGHT,

        //! Extend the selection right one word.
        WordRightExtend = XsciScintillaBase::SCI_WORDRIGHTEXTEND,

        //! Move to the end of the previous word.
        WordLeftEnd = XsciScintillaBase::SCI_WORDLEFTEND,

        //! Extend the selection to the end of the previous word.
        WordLeftEndExtend = XsciScintillaBase::SCI_WORDLEFTENDEXTEND,

        //! Move to the end of the next word.
        WordRightEnd = XsciScintillaBase::SCI_WORDRIGHTEND,

        //! Extend the selection to the end of the next word.
        WordRightEndExtend = XsciScintillaBase::SCI_WORDRIGHTENDEXTEND,

        //! Move left one word part.
        WordPartLeft = XsciScintillaBase::SCI_WORDPARTLEFT,

        //! Extend the selection left one word part.
        WordPartLeftExtend = XsciScintillaBase::SCI_WORDPARTLEFTEXTEND,

        //! Move right one word part.
        WordPartRight = XsciScintillaBase::SCI_WORDPARTRIGHT,

        //! Extend the selection right one word part.
        WordPartRightExtend = XsciScintillaBase::SCI_WORDPARTRIGHTEXTEND,

        //! Move to the start of the document line.
        Home = XsciScintillaBase::SCI_HOME,

        //! Extend the selection to the start of the document line.
        HomeExtend = XsciScintillaBase::SCI_HOMEEXTEND,

        //! Extend the rectangular selection to the start of the document line.
        HomeRectExtend = XsciScintillaBase::SCI_HOMERECTEXTEND,

        //! Move to the start of the displayed line.
        HomeDisplay = XsciScintillaBase::SCI_HOMEDISPLAY,

        //! Extend the selection to the start of the displayed line.
        HomeDisplayExtend = XsciScintillaBase::SCI_HOMEDISPLAYEXTEND,

        //! Move to the start of the displayed or document line.
        HomeWrap = XsciScintillaBase::SCI_HOMEWRAP,

        //! Extend the selection to the start of the displayed or document
        //! line.
        HomeWrapExtend = XsciScintillaBase::SCI_HOMEWRAPEXTEND,

        //! Move to the first visible character in the document line.
        VCHome = XsciScintillaBase::SCI_VCHOME,

        //! Extend the selection to the first visible character in the document
        //! line.
        VCHomeExtend = XsciScintillaBase::SCI_VCHOMEEXTEND,

        //! Extend the rectangular selection to the first visible character in
        //! the document line.
        VCHomeRectExtend = XsciScintillaBase::SCI_VCHOMERECTEXTEND,

        //! Move to the first visible character of the displayed or document
        //! line.
        VCHomeWrap = XsciScintillaBase::SCI_VCHOMEWRAP,

        //! Extend the selection to the first visible character of the
        //! displayed or document line.
        VCHomeWrapExtend = XsciScintillaBase::SCI_VCHOMEWRAPEXTEND,

        //! Move to the end of the document line.
        LineEnd = XsciScintillaBase::SCI_LINEEND,

        //! Extend the selection to the end of the document line.
        LineEndExtend = XsciScintillaBase::SCI_LINEENDEXTEND,

        //! Extend the rectangular selection to the end of the document line.
        LineEndRectExtend = XsciScintillaBase::SCI_LINEENDRECTEXTEND,

        //! Move to the end of the displayed line.
        LineEndDisplay = XsciScintillaBase::SCI_LINEENDDISPLAY,

        //! Extend the selection to the end of the displayed line.
        LineEndDisplayExtend = XsciScintillaBase::SCI_LINEENDDISPLAYEXTEND,

        //! Move to the end of the displayed or document line.
        LineEndWrap = XsciScintillaBase::SCI_LINEENDWRAP,

        //! Extend the selection to the end of the displayed or document line.
        LineEndWrapExtend = XsciScintillaBase::SCI_LINEENDWRAPEXTEND,

        //! Move to the start of the document.
        DocumentStart = XsciScintillaBase::SCI_DOCUMENTSTART,

        //! Extend the selection to the start of the document.
        DocumentStartExtend = XsciScintillaBase::SCI_DOCUMENTSTARTEXTEND,

        //! Move to the end of the document.
        DocumentEnd = XsciScintillaBase::SCI_DOCUMENTEND,

        //! Extend the selection to the end of the document.
        DocumentEndExtend = XsciScintillaBase::SCI_DOCUMENTENDEXTEND,

        //! Move up one page.
        PageUp = XsciScintillaBase::SCI_PAGEUP,

        //! Extend the selection up one page.
        PageUpExtend = XsciScintillaBase::SCI_PAGEUPEXTEND,

        //! Extend the rectangular selection up one page.
        PageUpRectExtend = XsciScintillaBase::SCI_PAGEUPRECTEXTEND,

        //! Move down one page.
        PageDown = XsciScintillaBase::SCI_PAGEDOWN,

        //! Extend the selection down one page.
        PageDownExtend = XsciScintillaBase::SCI_PAGEDOWNEXTEND,

        //! Extend the rectangular selection down one page.
        PageDownRectExtend = XsciScintillaBase::SCI_PAGEDOWNRECTEXTEND,

        //! Stuttered move up one page.
        StutteredPageUp = XsciScintillaBase::SCI_STUTTEREDPAGEUP,

        //! Stuttered extend the selection up one page.
        StutteredPageUpExtend = XsciScintillaBase::SCI_STUTTEREDPAGEUPEXTEND,

        //! Stuttered move down one page.
        StutteredPageDown = XsciScintillaBase::SCI_STUTTEREDPAGEDOWN,

        //! Stuttered extend the selection down one page.
        StutteredPageDownExtend = XsciScintillaBase::SCI_STUTTEREDPAGEDOWNEXTEND,

        //! Delete the current character.
        Delete = XsciScintillaBase::SCI_CLEAR,

        //! Delete the previous character.
        DeleteBack = XsciScintillaBase::SCI_DELETEBACK,

        //! Delete the previous character if not at start of line.
        DeleteBackNotLine = XsciScintillaBase::SCI_DELETEBACKNOTLINE,

        //! Delete the word to the left.
        DeleteWordLeft = XsciScintillaBase::SCI_DELWORDLEFT,

        //! Delete the word to the right.
        DeleteWordRight = XsciScintillaBase::SCI_DELWORDRIGHT,

        //! Delete right to the end of the next word.
        DeleteWordRightEnd = XsciScintillaBase::SCI_DELWORDRIGHTEND,

        //! Delete the line to the left.
        DeleteLineLeft = XsciScintillaBase::SCI_DELLINELEFT,

        //! Delete the line to the right.
        DeleteLineRight = XsciScintillaBase::SCI_DELLINERIGHT,

        //! Delete the current line.
        LineDelete = XsciScintillaBase::SCI_LINEDELETE,

        //! Cut the current line to the clipboard.
        LineCut = XsciScintillaBase::SCI_LINECUT,

        //! Copy the current line to the clipboard.
        LineCopy = XsciScintillaBase::SCI_LINECOPY,

        //! Transpose the current and previous lines.
        LineTranspose = XsciScintillaBase::SCI_LINETRANSPOSE,

        //! Duplicate the current line.
        LineDuplicate = XsciScintillaBase::SCI_LINEDUPLICATE,

        //! Select the whole document.
        SelectAll = XsciScintillaBase::SCI_SELECTALL,

        //! Move the selected lines up one line.
        MoveSelectedLinesUp = XsciScintillaBase::SCI_MOVESELECTEDLINESUP,

        //! Move the selected lines down one line.
        MoveSelectedLinesDown = XsciScintillaBase::SCI_MOVESELECTEDLINESDOWN,

        //! Duplicate the selection.
        SelectionDuplicate = XsciScintillaBase::SCI_SELECTIONDUPLICATE,

        //! Convert the selection to lower case.
        SelectionLowerCase = XsciScintillaBase::SCI_LOWERCASE,

        //! Convert the selection to upper case.
        SelectionUpperCase = XsciScintillaBase::SCI_UPPERCASE,

        //! Cut the selection to the clipboard.
        SelectionCut = XsciScintillaBase::SCI_CUT,

        //! Copy the selection to the clipboard.
        SelectionCopy = XsciScintillaBase::SCI_COPY,

        //! Paste from the clipboard.
        Paste = XsciScintillaBase::SCI_PASTE,

        //! Toggle insert/overtype.
        EditToggleOvertype = XsciScintillaBase::SCI_EDITTOGGLEOVERTYPE,

        //! Insert a platform dependent newline.
        Newline = XsciScintillaBase::SCI_NEWLINE,

        //! Insert a formfeed.
        Formfeed = XsciScintillaBase::SCI_FORMFEED,

        //! Indent one level.
        Tab = XsciScintillaBase::SCI_TAB,

        //! De-indent one level.
        Backtab = XsciScintillaBase::SCI_BACKTAB,

        //! Cancel any current operation.
        Cancel = XsciScintillaBase::SCI_CANCEL,

        //! Undo the last command.
        Undo = XsciScintillaBase::SCI_UNDO,

        //! Redo the last command.
        Redo = XsciScintillaBase::SCI_REDO,

        //! Zoom in.
        ZoomIn = XsciScintillaBase::SCI_ZOOMIN,

        //! Zoom out.
        ZoomOut = XsciScintillaBase::SCI_ZOOMOUT,

        //! Reverse the selected lines.
        ReverseLines = XsciScintillaBase::SCI_LINEREVERSE,
    };

    //! Return the command that will be executed by this instance.
    Command command() const {return scicmd;}

    //! Execute the command.
    void execute();

    //! Binds the key \a key to the command.  If \a key is 0 then the key
    //! binding is removed.  If \a key is invalid then the key binding is
    //! unchanged.  Valid keys are any visible or control character or any
    //! of \c Qt::Key_Down, \c Qt::Key_Up, \c Qt::Key_Left, \c Qt::Key_Right,
    //! \c Qt::Key_Home, \c Qt::Key_End, \c Qt::Key_PageUp,
    //! \c Qt::Key_PageDown, \c Qt::Key_Delete, \c Qt::Key_Insert,
    //! \c Qt::Key_Escape, \c Qt::Key_Backspace, \c Qt::Key_Tab,
    //! \c Qt::Key_Backtab, \c Qt::Key_Return, \c Qt::Key_Enter,
    //! \c Qt::Key_Super_L, \c Qt::Key_Super_R or \c Qt::Key_Menu.  Keys may be
    //! modified with any combination of \c Qt::ShiftModifier,
    //! \c Qt::ControlModifier, \c Qt::AltModifier and \c Qt::MetaModifier.
    //!
    //! \sa key(), setAlternateKey(), validKey()
    void setKey(int key);

    //! Binds the alternate key \a altkey to the command.  If \a key is 0
    //! then the alternate key binding is removed.
    //!
    //! \sa alternateKey(), setKey(), validKey()
    void setAlternateKey(int altkey);

    //! The key that is currently bound to the command is returned.
    //!
    //! \sa setKey(), alternateKey()
    int key() const {return qkey;}

    //! The alternate key that is currently bound to the command is
    //! returned.
    //!
    //! \sa setAlternateKey(), key()
    int alternateKey() const {return qaltkey;}

    //! If the key \a key is valid then true is returned.
    static bool validKey(int key);

    //! The user friendly description of the command is returned.
    QString description() const;

private:
    friend class XsciCommandSet;

    XsciCommand(XsciScintilla *qs, Command cmd, int key, int altkey,
            const char *desc);

    void bindKey(int key,int &qk,int &scik);

    XsciScintilla *qsCmd;
    Command scicmd;
    int qkey, scikey, qaltkey, scialtkey;
    const char *descCmd;

    XsciCommand(const XsciCommand &);
    XsciCommand &operator=(const XsciCommand &);
};

#endif
