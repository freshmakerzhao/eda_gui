// This module implements the XsciCommandSet class.
 


#include "Xsci/xscicommandset.h"

#include <QSettings>

#include "Xsci/xscicommand.h"
#include "Xsci/xsciscintilla.h"
#include "Xsci/xsciscintillabase.h"


// Starting with XScintilla v2.7 the standard OS/X keyboard shortcuts are used
// where possible.  In order to restore the behaviour of earlier versions then
// #define DONT_USE_OSX_KEYS here or add it to the qmake project (.pro) file.
#if defined(Q_OS_MAC) && !defined(DONT_USE_OSX_KEYS)
#define USING_OSX_KEYS
#else
#undef  USING_OSX_KEYS
#endif


// The ctor.
XsciCommandSet::XsciCommandSet(XsciScintilla *qs) : qsci(qs)
{
    struct sci_cmd {
        XsciCommand::Command cmd;
        int key;
        int altkey;
        const char *desc;
    };

    static struct sci_cmd cmd_table[] = {
        {
            XsciCommand::LineDown,
            Qt::Key_Down,
#if defined(USING_OSX_KEYS)
            Qt::Key_N | Qt::META,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("XsciCommand", "Move down one line")
        },
        {
            XsciCommand::LineDownExtend,
            Qt::Key_Down | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_N | Qt::META | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("XsciCommand", "Extend selection down one line")
        },
        {
            XsciCommand::LineDownRectExtend,
            Qt::Key_Down | Qt::ALT | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_N | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend rectangular selection down one line")
        },
        {
            XsciCommand::LineScrollDown,
            Qt::Key_Down | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Scroll view down one line")
        },
        {
            XsciCommand::LineUp,
            Qt::Key_Up,
#if defined(USING_OSX_KEYS)
            Qt::Key_P | Qt::META,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("XsciCommand", "Move up one line")
        },
        {
            XsciCommand::LineUpExtend,
            Qt::Key_Up | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_P | Qt::META | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("XsciCommand", "Extend selection up one line")
        },
        {
            XsciCommand::LineUpRectExtend,
            Qt::Key_Up | Qt::ALT | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_P | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend rectangular selection up one line")
        },
        {
            XsciCommand::LineScrollUp,
            Qt::Key_Up | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Scroll view up one line")
        },
        {
            XsciCommand::ScrollToStart,
#if defined(USING_OSX_KEYS)
            Qt::Key_Home,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Scroll to start of document")
        },
        {
            XsciCommand::ScrollToEnd,
#if defined(USING_OSX_KEYS)
            Qt::Key_End,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Scroll to end of document")
        },
        {
            XsciCommand::VerticalCentreCaret,
#if defined(USING_OSX_KEYS)
            Qt::Key_L | Qt::META,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Scroll vertically to centre current line")
        },
        {
            XsciCommand::ParaDown,
            Qt::Key_BracketRight | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Move down one paragraph")
        },
        {
            XsciCommand::ParaDownExtend,
            Qt::Key_BracketRight | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend selection down one paragraph")
        },
        {
            XsciCommand::ParaUp,
            Qt::Key_BracketLeft | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Move up one paragraph")
        },
        {
            XsciCommand::ParaUpExtend,
            Qt::Key_BracketLeft | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend selection up one paragraph")
        },
        {
            XsciCommand::CharLeft,
            Qt::Key_Left,
#if defined(USING_OSX_KEYS)
            Qt::Key_B | Qt::META,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("XsciCommand", "Move left one character")
        },
        {
            XsciCommand::CharLeftExtend,
            Qt::Key_Left | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_B | Qt::META | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend selection left one character")
        },
        {
            XsciCommand::CharLeftRectExtend,
            Qt::Key_Left | Qt::ALT | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_B | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend rectangular selection left one character")
        },
        {
            XsciCommand::CharRight,
            Qt::Key_Right,
#if defined(USING_OSX_KEYS)
            Qt::Key_F | Qt::META,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("XsciCommand", "Move right one character")
        },
        {
            XsciCommand::CharRightExtend,
            Qt::Key_Right | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_F | Qt::META | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend selection right one character")
        },
        {
            XsciCommand::CharRightRectExtend,
            Qt::Key_Right | Qt::ALT | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_F | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend rectangular selection right one character")
        },
        {
            XsciCommand::WordLeft,
#if defined(USING_OSX_KEYS)
            Qt::Key_Left | Qt::ALT,
#else
            Qt::Key_Left | Qt::CTRL,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Move left one word")
        },
        {
            XsciCommand::WordLeftExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_Left | Qt::ALT | Qt::SHIFT,
#else
            Qt::Key_Left | Qt::CTRL | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Extend selection left one word")
        },
        {
            XsciCommand::WordRight,
#if defined(USING_OSX_KEYS)
            0,
#else
            Qt::Key_Right | Qt::CTRL,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Move right one word")
        },
        {
            XsciCommand::WordRightExtend,
            Qt::Key_Right | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Extend selection right one word")
        },
        {
            XsciCommand::WordLeftEnd,
            0,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Move to end of previous word")
        },
        {
            XsciCommand::WordLeftEndExtend,
            0,
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend selection to end of previous word")
        },
        {
            XsciCommand::WordRightEnd,
#if defined(USING_OSX_KEYS)
            Qt::Key_Right | Qt::ALT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Move to end of next word")
        },
        {
            XsciCommand::WordRightEndExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_Right | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend selection to end of next word")
        },
        {
            XsciCommand::WordPartLeft,
            Qt::Key_Slash | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Move left one word part")
        },
        {
            XsciCommand::WordPartLeftExtend,
            Qt::Key_Slash | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend selection left one word part")
        },
        {
            XsciCommand::WordPartRight,
            Qt::Key_Backslash | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Move right one word part")
        },
        {
            XsciCommand::WordPartRightExtend,
            Qt::Key_Backslash | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend selection right one word part")
        },
        {
            XsciCommand::Home,
#if defined(USING_OSX_KEYS)
            Qt::Key_A | Qt::META,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Move to start of document line")
        },
        {
            XsciCommand::HomeExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_A | Qt::META | Qt::SHIFT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend selection to start of document line")
        },
        {
            XsciCommand::HomeRectExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_A | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend rectangular selection to start of document line")
        },
        {
            XsciCommand::HomeDisplay,
#if defined(USING_OSX_KEYS)
            Qt::Key_Left | Qt::CTRL,
#else
            Qt::Key_Home | Qt::ALT,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Move to start of display line")
        },
        {
            XsciCommand::HomeDisplayExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_Left | Qt::CTRL | Qt::SHIFT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend selection to start of display line")
        },
        {
            XsciCommand::HomeWrap,
            0,
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Move to start of display or document line")
        },
        {
            XsciCommand::HomeWrapExtend,
            0,
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend selection to start of display or document line")
        },
        {
            XsciCommand::VCHome,
#if defined(USING_OSX_KEYS)
            0,
#else
            Qt::Key_Home,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                "Move to first visible character in document line")
        },
        {
            XsciCommand::VCHomeExtend,
#if defined(USING_OSX_KEYS)
            0,
#else
            Qt::Key_Home | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                "Extend selection to first visible character in document line")
        },
        {
            XsciCommand::VCHomeRectExtend,
#if defined(USING_OSX_KEYS)
            0,
#else
            Qt::Key_Home | Qt::ALT | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                "Extend rectangular selection to first visible character in document line")
        },
        {
            XsciCommand::VCHomeWrap,
            0,
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Move to first visible character of display in document line")
        },
        {
            XsciCommand::VCHomeWrapExtend,
            0,
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend selection to first visible character in display or document line")
        },
        {
            XsciCommand::LineEnd,
#if defined(USING_OSX_KEYS)
            Qt::Key_E | Qt::META,
#else
            Qt::Key_End,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Move to end of document line")
        },
        {
            XsciCommand::LineEndExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_E | Qt::META | Qt::SHIFT,
#else
            Qt::Key_End | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend selection to end of document line")
        },
        {
            XsciCommand::LineEndRectExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_E | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            Qt::Key_End | Qt::ALT | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend rectangular selection to end of document line")
        },
        {
            XsciCommand::LineEndDisplay,
#if defined(USING_OSX_KEYS)
            Qt::Key_Right | Qt::CTRL,
#else
            Qt::Key_End | Qt::ALT,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Move to end of display line")
        },
        {
            XsciCommand::LineEndDisplayExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_Right | Qt::CTRL | Qt::SHIFT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend selection to end of display line")
        },
        {
            XsciCommand::LineEndWrap,
            0,
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Move to end of display or document line")
        },
        {
            XsciCommand::LineEndWrapExtend,
            0,
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend selection to end of display or document line")
        },
        {
            XsciCommand::DocumentStart,
#if defined(USING_OSX_KEYS)
            Qt::Key_Up | Qt::CTRL,
#else
            Qt::Key_Home | Qt::CTRL,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Move to start of document")
        },
        {
            XsciCommand::DocumentStartExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_Up | Qt::CTRL | Qt::SHIFT,
#else
            Qt::Key_Home | Qt::CTRL | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend selection to start of document")
        },
        {
            XsciCommand::DocumentEnd,
#if defined(USING_OSX_KEYS)
            Qt::Key_Down | Qt::CTRL,
#else
            Qt::Key_End | Qt::CTRL,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Move to end of document")
        },
        {
            XsciCommand::DocumentEndExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_Down | Qt::CTRL | Qt::SHIFT,
#else
            Qt::Key_End | Qt::CTRL | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend selection to end of document")
        },
        {
            XsciCommand::PageUp,
            Qt::Key_PageUp,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Move up one page")
        },
        {
            XsciCommand::PageUpExtend,
            Qt::Key_PageUp | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Extend selection up one page")
        },
        {
            XsciCommand::PageUpRectExtend,
            Qt::Key_PageUp | Qt::ALT | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend rectangular selection up one page")
        },
        {
            XsciCommand::PageDown,
            Qt::Key_PageDown,
#if defined(USING_OSX_KEYS)
            Qt::Key_V | Qt::META,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("XsciCommand", "Move down one page")
        },
        {
            XsciCommand::PageDownExtend,
            Qt::Key_PageDown | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_V | Qt::META | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("XsciCommand", "Extend selection down one page")
        },
        {
            XsciCommand::PageDownRectExtend,
            Qt::Key_PageDown | Qt::ALT | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_V | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Extend rectangular selection down one page")
        },
        {
            XsciCommand::StutteredPageUp,
            0,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Stuttered move up one page")
        },
        {
            XsciCommand::StutteredPageUpExtend,
            0,
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Stuttered extend selection up one page")
        },
        {
            XsciCommand::StutteredPageDown,
            0,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Stuttered move down one page")
        },
        {
            XsciCommand::StutteredPageDownExtend,
            0,
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Stuttered extend selection down one page")
        },
        {
            XsciCommand::Delete,
            Qt::Key_Delete,
#if defined(USING_OSX_KEYS)
            Qt::Key_D | Qt::META,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("XsciCommand", "Delete current character")
        },
        {
            XsciCommand::DeleteBack,
            Qt::Key_Backspace,
#if defined(USING_OSX_KEYS)
            Qt::Key_H | Qt::META,
#else
            Qt::Key_Backspace | Qt::SHIFT,
#endif
            QT_TRANSLATE_NOOP("XsciCommand", "Delete previous character")
        },
        {
            XsciCommand::DeleteBackNotLine,
            0,
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                "Delete previous character if not at start of line")
        },
        {
            XsciCommand::DeleteWordLeft,
            Qt::Key_Backspace | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Delete word to left")
        },
        {
            XsciCommand::DeleteWordRight,
            Qt::Key_Delete | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Delete word to right")
        },
        {
            XsciCommand::DeleteWordRightEnd,
#if defined(USING_OSX_KEYS)
            Qt::Key_Delete | Qt::ALT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Delete right to end of next word")
        },
        {
            XsciCommand::DeleteLineLeft,
            Qt::Key_Backspace | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Delete line to left")
        },
        {
            XsciCommand::DeleteLineRight,
#if defined(USING_OSX_KEYS)
            Qt::Key_K | Qt::META,
#else
            Qt::Key_Delete | Qt::CTRL | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Delete line to right")
        },
        {
            XsciCommand::LineDelete,
            Qt::Key_L | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Delete current line")
        },
        {
            XsciCommand::LineCut,
            Qt::Key_L | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Cut current line")
        },
        {
            XsciCommand::LineCopy,
            Qt::Key_T | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Copy current line")
        },
        {
            XsciCommand::LineTranspose,
            Qt::Key_T | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Transpose current and previous lines")
        },
        {
            XsciCommand::LineDuplicate,
            0,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Duplicate the current line")
        },
        {
            XsciCommand::SelectAll,
            Qt::Key_A | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Select all")
        },
        {
            XsciCommand::MoveSelectedLinesUp,
            0,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Move selected lines up one line")
        },
        {
            XsciCommand::MoveSelectedLinesDown,
            0,
            0,
            QT_TRANSLATE_NOOP("XsciCommand",
                    "Move selected lines down one line")
        },
        {
            XsciCommand::SelectionDuplicate,
            Qt::Key_D | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Duplicate selection")
        },
        {
            XsciCommand::SelectionLowerCase,
            Qt::Key_U | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Convert selection to lower case")
        },
        {
            XsciCommand::SelectionUpperCase,
            Qt::Key_U | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Convert selection to upper case")
        },
        {
            XsciCommand::SelectionCut,
            Qt::Key_X | Qt::CTRL,
            Qt::Key_Delete | Qt::SHIFT,
            QT_TRANSLATE_NOOP("XsciCommand", "Cut selection")
        },
        {
            XsciCommand::SelectionCopy,
            Qt::Key_C | Qt::CTRL,
            Qt::Key_Insert | Qt::CTRL,
            QT_TRANSLATE_NOOP("XsciCommand", "Copy selection")
        },
        {
            XsciCommand::Paste,
            Qt::Key_V | Qt::CTRL,
            Qt::Key_Insert | Qt::SHIFT,
            QT_TRANSLATE_NOOP("XsciCommand", "Paste")
        },
        {
            XsciCommand::EditToggleOvertype,
            Qt::Key_Insert,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Toggle insert/overtype")
        },
        {
            XsciCommand::Newline,
            Qt::Key_Return,
            Qt::Key_Return | Qt::SHIFT,
            QT_TRANSLATE_NOOP("XsciCommand", "Insert newline")
        },
        {
            XsciCommand::Formfeed,
            0,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Formfeed")
        },
        {
            XsciCommand::Tab,
            Qt::Key_Tab,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Indent one level")
        },
        {
            XsciCommand::Backtab,
            Qt::Key_Tab | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "De-indent one level")
        },
        {
            XsciCommand::Cancel,
            Qt::Key_Escape,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Cancel")
        },
        {
            XsciCommand::Undo,
            Qt::Key_Z | Qt::CTRL,
            Qt::Key_Backspace | Qt::ALT,
            QT_TRANSLATE_NOOP("XsciCommand", "Undo last command")
        },
        {
            XsciCommand::Redo,
#if defined(USING_OSX_KEYS)
            Qt::Key_Z | Qt::CTRL | Qt::SHIFT,
#else
            Qt::Key_Y | Qt::CTRL,
#endif
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Redo last command")
        },
        {
            XsciCommand::ZoomIn,
            Qt::Key_Plus | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Zoom in")
        },
        {
            XsciCommand::ZoomOut,
            Qt::Key_Minus | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("XsciCommand", "Zoom out")
        },
    };

    // Clear the default map.
    qsci->SendScintilla(XsciScintillaBase::SCI_CLEARALLCMDKEYS);

    // By default control characters don't do anything (rather than insert the
    // control character into the text).
    for (int k = 'A'; k <= 'Z'; ++k)
        qsci->SendScintilla(XsciScintillaBase::SCI_ASSIGNCMDKEY,
                k + (XsciScintillaBase::SCMOD_CTRL << 16),
                XsciScintillaBase::SCI_NULL);

    for (int i = 0; i < sizeof (cmd_table) / sizeof (cmd_table[0]); ++i)
        cmds.append(
                new XsciCommand(qsci, cmd_table[i].cmd, cmd_table[i].key,
                        cmd_table[i].altkey, cmd_table[i].desc));
}


// The dtor.
XsciCommandSet::~XsciCommandSet()
{
    for (int i = 0; i < cmds.count(); ++i)
        delete cmds.at(i);
}


// Read the command set from settings.
bool XsciCommandSet::readSettings(QSettings &qs, const char *prefix)
{
    bool rc = true;
    QString skey;

    for (int i = 0; i < cmds.count(); ++i)
    {
        XsciCommand *cmd = cmds.at(i);

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
bool XsciCommandSet::writeSettings(QSettings &qs, const char *prefix)
{
    bool rc = true;
    QString skey;

    for (int i = 0; i < cmds.count(); ++i)
    {
        XsciCommand *cmd = cmds.at(i);

        skey.sprintf("%s/keymap/c%d/", prefix,
                static_cast<int>(cmd->command()));

        // Write the key.
        qs.setValue(skey + "key", cmd->key());

        // Write the alternate key.
        qs.setValue(skey + "alt", cmd->alternateKey());
    }

    return rc;
}


// Clear the key bindings.
void XsciCommandSet::clearKeys()
{
    for (int i = 0; i < cmds.count(); ++i)
        cmds.at(i)->setKey(0);
}


// Clear the alternate key bindings.
void XsciCommandSet::clearAlternateKeys()
{
    for (int i = 0; i < cmds.count(); ++i)
        cmds.at(i)->setAlternateKey(0);
}


// Find the command bound to a key.
XsciCommand *XsciCommandSet::boundTo(int key) const
{
    for (int i = 0; i < cmds.count(); ++i)
    {
        XsciCommand *cmd = cmds.at(i);

        if (cmd->key() == key || cmd->alternateKey() == key)
            return cmd;
    }

    return 0;
}


// Find a command.
XsciCommand *XsciCommandSet::find(XsciCommand::Command command) const
{
    for (int i = 0; i < cmds.count(); ++i)
    {
        XsciCommand *cmd = cmds.at(i);

        if (cmd->command() == command)
            return cmd;
    }

    // This should never happen.
    return 0;
}
