// This module implements the ExsciCommandSet class.



#include "Exsci/exscicommandset.h"

#include <QSettings>

#include "Exsci/exscicommand.h"
#include "Exsci/exsciscintilla.h"
#include "Exsci/exsciscintillabase.h"


// Starting with EXScintilla v2.7 the standard OS/X keyboard shortcuts are used
// where possible.  In order to restore the behaviour of earlier versions then
// #define DONT_USE_OSX_KEYS here or add it to the qmake project (.pro) file.
#if defined(Q_OS_MAC) && !defined(DONT_USE_OSX_KEYS)
#define USING_OSX_KEYS
#else
#undef  USING_OSX_KEYS
#endif


// The ctor.
ExsciCommandSet::ExsciCommandSet(ExsciScintilla *qs) : exsci(qs)
{
    struct sci_cmd {
        ExsciCommand::Command cmd;
        int key;
        int altkey;
        const char *desc;
    };

    static struct sci_cmd cmd_table[] = {
        {
            ExsciCommand::LineDown,
            Qt::Key_Down,
#if defined(USING_OSX_KEYS)
            Qt::Key_N | Qt::META,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("ExsciCommand", "Move down one line")
        },
        {
            ExsciCommand::LineDownExtend,
            Qt::Key_Down | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_N | Qt::META | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("ExsciCommand", "Extend selection down one line")
        },
        {
            ExsciCommand::LineDownRectExtend,
            Qt::Key_Down | Qt::ALT | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_N | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend rectangular selection down one line")
        },
        {
            ExsciCommand::LineScrollDown,
            Qt::Key_Down | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Scroll view down one line")
        },
        {
            ExsciCommand::LineUp,
            Qt::Key_Up,
#if defined(USING_OSX_KEYS)
            Qt::Key_P | Qt::META,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("ExsciCommand", "Move up one line")
        },
        {
            ExsciCommand::LineUpExtend,
            Qt::Key_Up | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_P | Qt::META | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("ExsciCommand", "Extend selection up one line")
        },
        {
            ExsciCommand::LineUpRectExtend,
            Qt::Key_Up | Qt::ALT | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_P | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend rectangular selection up one line")
        },
        {
            ExsciCommand::LineScrollUp,
            Qt::Key_Up | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Scroll view up one line")
        },
        {
            ExsciCommand::ScrollToStart,
#if defined(USING_OSX_KEYS)
            Qt::Key_Home,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Scroll to start of document")
        },
        {
            ExsciCommand::ScrollToEnd,
#if defined(USING_OSX_KEYS)
            Qt::Key_End,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Scroll to end of document")
        },
        {
            ExsciCommand::VerticalCentreCaret,
#if defined(USING_OSX_KEYS)
            Qt::Key_L | Qt::META,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Scroll vertically to centre current line")
        },
        {
            ExsciCommand::ParaDown,
            Qt::Key_BracketRight | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Move down one paragraph")
        },
        {
            ExsciCommand::ParaDownExtend,
            Qt::Key_BracketRight | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend selection down one paragraph")
        },
        {
            ExsciCommand::ParaUp,
            Qt::Key_BracketLeft | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Move up one paragraph")
        },
        {
            ExsciCommand::ParaUpExtend,
            Qt::Key_BracketLeft | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend selection up one paragraph")
        },
        {
            ExsciCommand::CharLeft,
            Qt::Key_Left,
#if defined(USING_OSX_KEYS)
            Qt::Key_B | Qt::META,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("ExsciCommand", "Move left one character")
        },
        {
            ExsciCommand::CharLeftExtend,
            Qt::Key_Left | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_B | Qt::META | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend selection left one character")
        },
        {
            ExsciCommand::CharLeftRectExtend,
            Qt::Key_Left | Qt::ALT | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_B | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend rectangular selection left one character")
        },
        {
            ExsciCommand::CharRight,
            Qt::Key_Right,
#if defined(USING_OSX_KEYS)
            Qt::Key_F | Qt::META,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("ExsciCommand", "Move right one character")
        },
        {
            ExsciCommand::CharRightExtend,
            Qt::Key_Right | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_F | Qt::META | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend selection right one character")
        },
        {
            ExsciCommand::CharRightRectExtend,
            Qt::Key_Right | Qt::ALT | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_F | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend rectangular selection right one character")
        },
        {
            ExsciCommand::WordLeft,
#if defined(USING_OSX_KEYS)
            Qt::Key_Left | Qt::ALT,
#else
            Qt::Key_Left | Qt::CTRL,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Move left one word")
        },
        {
            ExsciCommand::WordLeftExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_Left | Qt::ALT | Qt::SHIFT,
#else
            Qt::Key_Left | Qt::CTRL | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Extend selection left one word")
        },
        {
            ExsciCommand::WordRight,
#if defined(USING_OSX_KEYS)
            0,
#else
            Qt::Key_Right | Qt::CTRL,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Move right one word")
        },
        {
            ExsciCommand::WordRightExtend,
            Qt::Key_Right | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Extend selection right one word")
        },
        {
            ExsciCommand::WordLeftEnd,
            0,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Move to end of previous word")
        },
        {
            ExsciCommand::WordLeftEndExtend,
            0,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend selection to end of previous word")
        },
        {
            ExsciCommand::WordRightEnd,
#if defined(USING_OSX_KEYS)
            Qt::Key_Right | Qt::ALT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Move to end of next word")
        },
        {
            ExsciCommand::WordRightEndExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_Right | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend selection to end of next word")
        },
        {
            ExsciCommand::WordPartLeft,
            Qt::Key_Slash | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Move left one word part")
        },
        {
            ExsciCommand::WordPartLeftExtend,
            Qt::Key_Slash | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend selection left one word part")
        },
        {
            ExsciCommand::WordPartRight,
            Qt::Key_Backslash | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Move right one word part")
        },
        {
            ExsciCommand::WordPartRightExtend,
            Qt::Key_Backslash | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend selection right one word part")
        },
        {
            ExsciCommand::Home,
#if defined(USING_OSX_KEYS)
            Qt::Key_A | Qt::META,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Move to start of document line")
        },
        {
            ExsciCommand::HomeExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_A | Qt::META | Qt::SHIFT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend selection to start of document line")
        },
        {
            ExsciCommand::HomeRectExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_A | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend rectangular selection to start of document line")
        },
        {
            ExsciCommand::HomeDisplay,
#if defined(USING_OSX_KEYS)
            Qt::Key_Left | Qt::CTRL,
#else
            Qt::Key_Home | Qt::ALT,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Move to start of display line")
        },
        {
            ExsciCommand::HomeDisplayExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_Left | Qt::CTRL | Qt::SHIFT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend selection to start of display line")
        },
        {
            ExsciCommand::HomeWrap,
            0,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Move to start of display or document line")
        },
        {
            ExsciCommand::HomeWrapExtend,
            0,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend selection to start of display or document line")
        },
        {
            ExsciCommand::VCHome,
#if defined(USING_OSX_KEYS)
            0,
#else
            Qt::Key_Home,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                "Move to first visible character in document line")
        },
        {
            ExsciCommand::VCHomeExtend,
#if defined(USING_OSX_KEYS)
            0,
#else
            Qt::Key_Home | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                "Extend selection to first visible character in document line")
        },
        {
            ExsciCommand::VCHomeRectExtend,
#if defined(USING_OSX_KEYS)
            0,
#else
            Qt::Key_Home | Qt::ALT | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                "Extend rectangular selection to first visible character in document line")
        },
        {
            ExsciCommand::VCHomeWrap,
            0,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Move to first visible character of display in document line")
        },
        {
            ExsciCommand::VCHomeWrapExtend,
            0,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend selection to first visible character in display or document line")
        },
        {
            ExsciCommand::LineEnd,
#if defined(USING_OSX_KEYS)
            Qt::Key_E | Qt::META,
#else
            Qt::Key_End,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Move to end of document line")
        },
        {
            ExsciCommand::LineEndExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_E | Qt::META | Qt::SHIFT,
#else
            Qt::Key_End | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend selection to end of document line")
        },
        {
            ExsciCommand::LineEndRectExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_E | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            Qt::Key_End | Qt::ALT | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend rectangular selection to end of document line")
        },
        {
            ExsciCommand::LineEndDisplay,
#if defined(USING_OSX_KEYS)
            Qt::Key_Right | Qt::CTRL,
#else
            Qt::Key_End | Qt::ALT,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Move to end of display line")
        },
        {
            ExsciCommand::LineEndDisplayExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_Right | Qt::CTRL | Qt::SHIFT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend selection to end of display line")
        },
        {
            ExsciCommand::LineEndWrap,
            0,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Move to end of display or document line")
        },
        {
            ExsciCommand::LineEndWrapExtend,
            0,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend selection to end of display or document line")
        },
        {
            ExsciCommand::DocumentStart,
#if defined(USING_OSX_KEYS)
            Qt::Key_Up | Qt::CTRL,
#else
            Qt::Key_Home | Qt::CTRL,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Move to start of document")
        },
        {
            ExsciCommand::DocumentStartExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_Up | Qt::CTRL | Qt::SHIFT,
#else
            Qt::Key_Home | Qt::CTRL | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend selection to start of document")
        },
        {
            ExsciCommand::DocumentEnd,
#if defined(USING_OSX_KEYS)
            Qt::Key_Down | Qt::CTRL,
#else
            Qt::Key_End | Qt::CTRL,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Move to end of document")
        },
        {
            ExsciCommand::DocumentEndExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_Down | Qt::CTRL | Qt::SHIFT,
#else
            Qt::Key_End | Qt::CTRL | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend selection to end of document")
        },
        {
            ExsciCommand::PageUp,
            Qt::Key_PageUp,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Move up one page")
        },
        {
            ExsciCommand::PageUpExtend,
            Qt::Key_PageUp | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Extend selection up one page")
        },
        {
            ExsciCommand::PageUpRectExtend,
            Qt::Key_PageUp | Qt::ALT | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend rectangular selection up one page")
        },
        {
            ExsciCommand::PageDown,
            Qt::Key_PageDown,
#if defined(USING_OSX_KEYS)
            Qt::Key_V | Qt::META,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("ExsciCommand", "Move down one page")
        },
        {
            ExsciCommand::PageDownExtend,
            Qt::Key_PageDown | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_V | Qt::META | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("ExsciCommand", "Extend selection down one page")
        },
        {
            ExsciCommand::PageDownRectExtend,
            Qt::Key_PageDown | Qt::ALT | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_V | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Extend rectangular selection down one page")
        },
        {
            ExsciCommand::StutteredPageUp,
            0,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Stuttered move up one page")
        },
        {
            ExsciCommand::StutteredPageUpExtend,
            0,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Stuttered extend selection up one page")
        },
        {
            ExsciCommand::StutteredPageDown,
            0,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Stuttered move down one page")
        },
        {
            ExsciCommand::StutteredPageDownExtend,
            0,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Stuttered extend selection down one page")
        },
        {
            ExsciCommand::Delete,
            Qt::Key_Delete,
#if defined(USING_OSX_KEYS)
            Qt::Key_D | Qt::META,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("ExsciCommand", "Delete current character")
        },
        {
            ExsciCommand::DeleteBack,
            Qt::Key_Backspace,
#if defined(USING_OSX_KEYS)
            Qt::Key_H | Qt::META,
#else
            Qt::Key_Backspace | Qt::SHIFT,
#endif
            QT_TRANSLATE_NOOP("ExsciCommand", "Delete previous character")
        },
        {
            ExsciCommand::DeleteBackNotLine,
            0,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                "Delete previous character if not at start of line")
        },
        {
            ExsciCommand::DeleteWordLeft,
            Qt::Key_Backspace | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Delete word to left")
        },
        {
            ExsciCommand::DeleteWordRight,
            Qt::Key_Delete | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Delete word to right")
        },
        {
            ExsciCommand::DeleteWordRightEnd,
#if defined(USING_OSX_KEYS)
            Qt::Key_Delete | Qt::ALT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Delete right to end of next word")
        },
        {
            ExsciCommand::DeleteLineLeft,
            Qt::Key_Backspace | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Delete line to left")
        },
        {
            ExsciCommand::DeleteLineRight,
#if defined(USING_OSX_KEYS)
            Qt::Key_K | Qt::META,
#else
            Qt::Key_Delete | Qt::CTRL | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Delete line to right")
        },
        {
            ExsciCommand::LineDelete,
            Qt::Key_L | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Delete current line")
        },
        {
            ExsciCommand::LineCut,
            Qt::Key_L | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Cut current line")
        },
        {
            ExsciCommand::LineCopy,
            Qt::Key_T | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Copy current line")
        },
        {
            ExsciCommand::LineTranspose,
            Qt::Key_T | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Transpose current and previous lines")
        },
        {
            ExsciCommand::LineDuplicate,
            0,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Duplicate the current line")
        },
        {
            ExsciCommand::SelectAll,
            Qt::Key_A | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Select all")
        },
        {
            ExsciCommand::MoveSelectedLinesUp,
            0,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Move selected lines up one line")
        },
        {
            ExsciCommand::MoveSelectedLinesDown,
            0,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand",
                    "Move selected lines down one line")
        },
        {
            ExsciCommand::SelectionDuplicate,
            Qt::Key_D | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Duplicate selection")
        },
        {
            ExsciCommand::SelectionLowerCase,
            Qt::Key_U | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Convert selection to lower case")
        },
        {
            ExsciCommand::SelectionUpperCase,
            Qt::Key_U | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Convert selection to upper case")
        },
        {
            ExsciCommand::SelectionCut,
            Qt::Key_X | Qt::CTRL,
            Qt::Key_Delete | Qt::SHIFT,
            QT_TRANSLATE_NOOP("ExsciCommand", "Cut selection")
        },
        {
            ExsciCommand::SelectionCopy,
            Qt::Key_C | Qt::CTRL,
            Qt::Key_Insert | Qt::CTRL,
            QT_TRANSLATE_NOOP("ExsciCommand", "Copy selection")
        },
        {
            ExsciCommand::Paste,
            Qt::Key_V | Qt::CTRL,
            Qt::Key_Insert | Qt::SHIFT,
            QT_TRANSLATE_NOOP("ExsciCommand", "Paste")
        },
        {
            ExsciCommand::EditToggleOvertype,
            Qt::Key_Insert,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Toggle insert/overtype")
        },
        {
            ExsciCommand::Newline,
            Qt::Key_Return,
            Qt::Key_Return | Qt::SHIFT,
            QT_TRANSLATE_NOOP("ExsciCommand", "Insert newline")
        },
        {
            ExsciCommand::Formfeed,
            0,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Formfeed")
        },
        {
            ExsciCommand::Tab,
            Qt::Key_Tab,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Indent one level")
        },
        {
            ExsciCommand::Backtab,
            Qt::Key_Tab | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "De-indent one level")
        },
        {
            ExsciCommand::Cancel,
            Qt::Key_Escape,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Cancel")
        },
        {
            ExsciCommand::Undo,
            Qt::Key_Z | Qt::CTRL,
            Qt::Key_Backspace | Qt::ALT,
            QT_TRANSLATE_NOOP("ExsciCommand", "Undo last command")
        },
        {
            ExsciCommand::Redo,
#if defined(USING_OSX_KEYS)
            Qt::Key_Z | Qt::CTRL | Qt::SHIFT,
#else
            Qt::Key_Y | Qt::CTRL,
#endif
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Redo last command")
        },
        {
            ExsciCommand::ZoomIn,
            Qt::Key_Plus | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Zoom in")
        },
        {
            ExsciCommand::ZoomOut,
            Qt::Key_Minus | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("ExsciCommand", "Zoom out")
        },
    };

    // Clear the default map.
    exsci->SendScintilla(ExsciScintillaBase::SCI_CLEARALLCMDKEYS);

    // By default control characters don't do anything (rather than insert the
    // control character into the text).
    for (int k = 'A'; k <= 'Z'; ++k)
        exsci->SendScintilla(ExsciScintillaBase::SCI_ASSIGNCMDKEY,
                k + (ExsciScintillaBase::SCMOD_CTRL << 16),
                ExsciScintillaBase::SCI_NULL);

    for (int i = 0; i < sizeof (cmd_table) / sizeof (cmd_table[0]); ++i)
        cmds.append(
                new ExsciCommand(exsci, cmd_table[i].cmd, cmd_table[i].key,
                        cmd_table[i].altkey, cmd_table[i].desc));
}


// The dtor.
ExsciCommandSet::~ExsciCommandSet()
{
    for (int i = 0; i < cmds.count(); ++i)
        delete cmds.at(i);
}


// Read the command set from settings.
bool ExsciCommandSet::readSettings(QSettings &qs, const char *prefix)
{
    bool rc = true;
    QString skey;

    for (int i = 0; i < cmds.count(); ++i)
    {
        ExsciCommand *cmd = cmds.at(i);

        skey.sprintf("%s/keymap/c%d/", prefix,
                static_cast<int>(cmd->command()));

        int key;
        bool ok;

        // Read the key.
        ok = qs.contains(skey + "key");
        key = qs.value(skey + "key", 0).toInt();

        if (ok)
            cmd->setKey(key);
        else
            rc = false;

        // Read the alternate key.
        ok = qs.contains(skey + "alt");
        key = qs.value(skey + "alt", 0).toInt();

        if (ok)
            cmd->setAlternateKey(key);
        else
            rc = false;
    }

    return rc;
}


// Write the command set to settings.
bool ExsciCommandSet::writeSettings(QSettings &qs, const char *prefix)
{
    bool rc = true;
    QString skey;

    for (int i = 0; i < cmds.count(); ++i)
    {
        ExsciCommand *cmd = cmds.at(i);

        skey.sprintf("%s/keymap/c%d/", prefix,
                static_cast<int>(cmd->command()));

        // Write the key.
        qs.setValue(skey + "key", cmd->key());

        // Write the alternate key.
        qs.setValue(skey + "alt", cmd->key());
    }

    return rc;
}


// Clear the key bindings.
void ExsciCommandSet::clearKeys()
{
    for (int i = 0; i < cmds.count(); ++i)
        cmds.at(i)->setKey(0);
}


// Clear the alternate key bindings.
void ExsciCommandSet::clearAlternateKeys()
{
    for (int i = 0; i < cmds.count(); ++i)
        cmds.at(i)->setAlternateKey(0);
}


// Find the command bound to a key.
ExsciCommand *ExsciCommandSet::boundTo(int key) const
{
    for (int i = 0; i < cmds.count(); ++i)
    {
        ExsciCommand *cmd = cmds.at(i);

        if (cmd->key() == key || cmd->alternateKey() == key)
            return cmd;
    }

    return 0;
}


// Find a command.
ExsciCommand *ExsciCommandSet::find(ExsciCommand::Command command) const
{
    for (int i = 0; i < cmds.count(); ++i)
    {
        ExsciCommand *cmd = cmds.at(i);

        if (cmd->command() == command)
            return cmd;
    }

    // This should never happen.
    return 0;
}
