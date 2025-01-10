// This defines the interface to the ExsciCommand class.



#ifndef EXSCICOMMAND_H
#define EXSCICOMMAND_H

#include <qstring.h>

#include <Exsci/exsciglobal.h>
#include <Exsci/exsciscintillabase.h>


class ExsciScintilla;


//! \brief The ExsciCommand class represents an internal editor command that may
//! have one or two keys bound to it.
//!
//! Methods are provided to change the keys bound to the command and to remove
//! a key binding.  Each command has a user friendly description of the command
//! for use in key mapping dialogs.
class EXSCINTILLA_EXPORT ExsciCommand
{
public:
    //! This enum defines the different commands that can be assigned to a key.
    enum Command {
        //! Move down one line.
        LineDown = ExsciScintillaBase::SCI_LINEDOWN,

        //! Extend the selection down one line.
        LineDownExtend = ExsciScintillaBase::SCI_LINEDOWNEXTEND,

        //! Extend the rectangular selection down one line.
        LineDownRectExtend = ExsciScintillaBase::SCI_LINEDOWNRECTEXTEND,

        //! Scroll the view down one line.
        LineScrollDown = ExsciScintillaBase::SCI_LINESCROLLDOWN,

        //! Move up one line.
        LineUp = ExsciScintillaBase::SCI_LINEUP,

        //! Extend the selection up one line.
        LineUpExtend = ExsciScintillaBase::SCI_LINEUPEXTEND,

        //! Extend the rectangular selection up one line.
        LineUpRectExtend = ExsciScintillaBase::SCI_LINEUPRECTEXTEND,

        //! Scroll the view up one line.
        LineScrollUp = ExsciScintillaBase::SCI_LINESCROLLUP,

        //! Scroll to the start of the document.
        ScrollToStart = ExsciScintillaBase::SCI_SCROLLTOSTART,

        //! Scroll to the end of the document.
        ScrollToEnd = ExsciScintillaBase::SCI_SCROLLTOEND,

        //! Scroll vertically to centre the current line.
        VerticalCentreCaret = ExsciScintillaBase::SCI_VERTICALCENTRECARET,

        //! Move down one paragraph.
        ParaDown = ExsciScintillaBase::SCI_PARADOWN,

        //! Extend the selection down one paragraph.
        ParaDownExtend = ExsciScintillaBase::SCI_PARADOWNEXTEND,

        //! Move up one paragraph.
        ParaUp = ExsciScintillaBase::SCI_PARAUP,

        //! Extend the selection up one paragraph.
        ParaUpExtend = ExsciScintillaBase::SCI_PARAUPEXTEND,

        //! Move left one character.
        CharLeft = ExsciScintillaBase::SCI_CHARLEFT,

        //! Extend the selection left one character.
        CharLeftExtend = ExsciScintillaBase::SCI_CHARLEFTEXTEND,

        //! Extend the rectangular selection left one character.
        CharLeftRectExtend = ExsciScintillaBase::SCI_CHARLEFTRECTEXTEND,

        //! Move right one character.
        CharRight = ExsciScintillaBase::SCI_CHARRIGHT,

        //! Extend the selection right one character.
        CharRightExtend = ExsciScintillaBase::SCI_CHARRIGHTEXTEND,

        //! Extend the rectangular selection right one character.
        CharRightRectExtend = ExsciScintillaBase::SCI_CHARRIGHTRECTEXTEND,

        //! Move left one word.
        WordLeft = ExsciScintillaBase::SCI_WORDLEFT,

        //! Extend the selection left one word.
        WordLeftExtend = ExsciScintillaBase::SCI_WORDLEFTEXTEND,

        //! Move right one word.
        WordRight = ExsciScintillaBase::SCI_WORDRIGHT,

        //! Extend the selection right one word.
        WordRightExtend = ExsciScintillaBase::SCI_WORDRIGHTEXTEND,

        //! Move to the end of the previous word.
        WordLeftEnd = ExsciScintillaBase::SCI_WORDLEFTEND,

        //! Extend the selection to the end of the previous word.
        WordLeftEndExtend = ExsciScintillaBase::SCI_WORDLEFTENDEXTEND,

        //! Move to the end of the next word.
        WordRightEnd = ExsciScintillaBase::SCI_WORDRIGHTEND,

        //! Extend the selection to the end of the next word.
        WordRightEndExtend = ExsciScintillaBase::SCI_WORDRIGHTENDEXTEND,

        //! Move left one word part.
        WordPartLeft = ExsciScintillaBase::SCI_WORDPARTLEFT,

        //! Extend the selection left one word part.
        WordPartLeftExtend = ExsciScintillaBase::SCI_WORDPARTLEFTEXTEND,

        //! Move right one word part.
        WordPartRight = ExsciScintillaBase::SCI_WORDPARTRIGHT,

        //! Extend the selection right one word part.
        WordPartRightExtend = ExsciScintillaBase::SCI_WORDPARTRIGHTEXTEND,

        //! Move to the start of the document line.
        Home = ExsciScintillaBase::SCI_HOME,

        //! Extend the selection to the start of the document line.
        HomeExtend = ExsciScintillaBase::SCI_HOMEEXTEND,

        //! Extend the rectangular selection to the start of the document line.
        HomeRectExtend = ExsciScintillaBase::SCI_HOMERECTEXTEND,

        //! Move to the start of the displayed line.
        HomeDisplay = ExsciScintillaBase::SCI_HOMEDISPLAY,

        //! Extend the selection to the start of the displayed line.
        HomeDisplayExtend = ExsciScintillaBase::SCI_HOMEDISPLAYEXTEND,

        //! Move to the start of the displayed or document line.
        HomeWrap = ExsciScintillaBase::SCI_HOMEWRAP,

        //! Extend the selection to the start of the displayed or document
        //! line.
        HomeWrapExtend = ExsciScintillaBase::SCI_HOMEWRAPEXTEND,

        //! Move to the first visible character in the document line.
        VCHome = ExsciScintillaBase::SCI_VCHOME,

        //! Extend the selection to the first visible character in the document
        //! line.
        VCHomeExtend = ExsciScintillaBase::SCI_VCHOMEEXTEND,

        //! Extend the rectangular selection to the first visible character in
        //! the document line.
        VCHomeRectExtend = ExsciScintillaBase::SCI_VCHOMERECTEXTEND,

        //! Move to the first visible character of the displayed or document
        //! line.
        VCHomeWrap = ExsciScintillaBase::SCI_VCHOMEWRAP,

        //! Extend the selection to the first visible character of the
        //! displayed or document line.
        VCHomeWrapExtend = ExsciScintillaBase::SCI_VCHOMEWRAPEXTEND,

        //! Move to the end of the document line.
        LineEnd = ExsciScintillaBase::SCI_LINEEND,

        //! Extend the selection to the end of the document line.
        LineEndExtend = ExsciScintillaBase::SCI_LINEENDEXTEND,

        //! Extend the rectangular selection to the end of the document line.
        LineEndRectExtend = ExsciScintillaBase::SCI_LINEENDRECTEXTEND,

        //! Move to the end of the displayed line.
        LineEndDisplay = ExsciScintillaBase::SCI_LINEENDDISPLAY,

        //! Extend the selection to the end of the displayed line.
        LineEndDisplayExtend = ExsciScintillaBase::SCI_LINEENDDISPLAYEXTEND,

        //! Move to the end of the displayed or document line.
        LineEndWrap = ExsciScintillaBase::SCI_LINEENDWRAP,

        //! Extend the selection to the end of the displayed or document line.
        LineEndWrapExtend = ExsciScintillaBase::SCI_LINEENDWRAPEXTEND,

        //! Move to the start of the document.
        DocumentStart = ExsciScintillaBase::SCI_DOCUMENTSTART,

        //! Extend the selection to the start of the document.
        DocumentStartExtend = ExsciScintillaBase::SCI_DOCUMENTSTARTEXTEND,

        //! Move to the end of the document.
        DocumentEnd = ExsciScintillaBase::SCI_DOCUMENTEND,

        //! Extend the selection to the end of the document.
        DocumentEndExtend = ExsciScintillaBase::SCI_DOCUMENTENDEXTEND,

        //! Move up one page.
        PageUp = ExsciScintillaBase::SCI_PAGEUP,

        //! Extend the selection up one page.
        PageUpExtend = ExsciScintillaBase::SCI_PAGEUPEXTEND,

        //! Extend the rectangular selection up one page.
        PageUpRectExtend = ExsciScintillaBase::SCI_PAGEUPRECTEXTEND,

        //! Move down one page.
        PageDown = ExsciScintillaBase::SCI_PAGEDOWN,

        //! Extend the selection down one page.
        PageDownExtend = ExsciScintillaBase::SCI_PAGEDOWNEXTEND,

        //! Extend the rectangular selection down one page.
        PageDownRectExtend = ExsciScintillaBase::SCI_PAGEDOWNRECTEXTEND,

        //! Stuttered move up one page.
        StutteredPageUp = ExsciScintillaBase::SCI_STUTTEREDPAGEUP,

        //! Stuttered extend the selection up one page.
        StutteredPageUpExtend = ExsciScintillaBase::SCI_STUTTEREDPAGEUPEXTEND,

        //! Stuttered move down one page.
        StutteredPageDown = ExsciScintillaBase::SCI_STUTTEREDPAGEDOWN,

        //! Stuttered extend the selection down one page.
        StutteredPageDownExtend = ExsciScintillaBase::SCI_STUTTEREDPAGEDOWNEXTEND,

        //! Delete the current character.
        Delete = ExsciScintillaBase::SCI_CLEAR,

        //! Delete the previous character.
        DeleteBack = ExsciScintillaBase::SCI_DELETEBACK,

        //! Delete the previous character if not at start of line.
        DeleteBackNotLine = ExsciScintillaBase::SCI_DELETEBACKNOTLINE,

        //! Delete the word to the left.
        DeleteWordLeft = ExsciScintillaBase::SCI_DELWORDLEFT,

        //! Delete the word to the right.
        DeleteWordRight = ExsciScintillaBase::SCI_DELWORDRIGHT,

        //! Delete right to the end of the next word.
        DeleteWordRightEnd = ExsciScintillaBase::SCI_DELWORDRIGHTEND,

        //! Delete the line to the left.
        DeleteLineLeft = ExsciScintillaBase::SCI_DELLINELEFT,

        //! Delete the line to the right.
        DeleteLineRight = ExsciScintillaBase::SCI_DELLINERIGHT,

        //! Delete the current line.
        LineDelete = ExsciScintillaBase::SCI_LINEDELETE,

        //! Cut the current line to the clipboard.
        LineCut = ExsciScintillaBase::SCI_LINECUT,

        //! Copy the current line to the clipboard.
        LineCopy = ExsciScintillaBase::SCI_LINECOPY,

        //! Transpose the current and previous lines.
        LineTranspose = ExsciScintillaBase::SCI_LINETRANSPOSE,

        //! Duplicate the current line.
        LineDuplicate = ExsciScintillaBase::SCI_LINEDUPLICATE,

        //! Select the whole document.
        SelectAll = ExsciScintillaBase::SCI_SELECTALL,

        //! Move the selected lines up one line.
        MoveSelectedLinesUp = ExsciScintillaBase::SCI_MOVESELECTEDLINESUP,

        //! Move the selected lines down one line.
        MoveSelectedLinesDown = ExsciScintillaBase::SCI_MOVESELECTEDLINESDOWN,

        //! Duplicate the selection.
        SelectionDuplicate = ExsciScintillaBase::SCI_SELECTIONDUPLICATE,

        //! Convert the selection to lower case.
        SelectionLowerCase = ExsciScintillaBase::SCI_LOWERCASE,

        //! Convert the selection to upper case.
        SelectionUpperCase = ExsciScintillaBase::SCI_UPPERCASE,

        //! Cut the selection to the clipboard.
        SelectionCut = ExsciScintillaBase::SCI_CUT,

        //! Copy the selection to the clipboard.
        SelectionCopy = ExsciScintillaBase::SCI_COPY,

        //! Paste from the clipboard.
        Paste = ExsciScintillaBase::SCI_PASTE,

        //! Toggle insert/overtype.
        EditToggleOvertype = ExsciScintillaBase::SCI_EDITTOGGLEOVERTYPE,

        //! Insert a platform dependent newline.
        Newline = ExsciScintillaBase::SCI_NEWLINE,

        //! Insert a formfeed.
        Formfeed = ExsciScintillaBase::SCI_FORMFEED,

        //! Indent one level.
        Tab = ExsciScintillaBase::SCI_TAB,

        //! De-indent one level.
        Backtab = ExsciScintillaBase::SCI_BACKTAB,

        //! Cancel any current operation.
        Cancel = ExsciScintillaBase::SCI_CANCEL,

        //! Undo the last command.
        Undo = ExsciScintillaBase::SCI_UNDO,

        //! Redo the last command.
        Redo = ExsciScintillaBase::SCI_REDO,

        //! Zoom in.
        ZoomIn = ExsciScintillaBase::SCI_ZOOMIN,

        //! Zoom out.
        ZoomOut = ExsciScintillaBase::SCI_ZOOMOUT,
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
    friend class ExsciCommandSet;

    ExsciCommand(ExsciScintilla *qs, Command cmd, int key, int altkey,
            const char *desc);

    void bindKey(int key,int &qk,int &scik);

    ExsciScintilla *qsCmd;
    Command scicmd;
    int qkey, scikey, qaltkey, scialtkey;
    const char *descCmd;

    ExsciCommand(const ExsciCommand &);
    ExsciCommand &operator=(const ExsciCommand &);
};

#endif
