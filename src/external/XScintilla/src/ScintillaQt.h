// The definition of the Qt specific subclass of ScintillaBase.
 


#ifndef SCINTILLAQT_H
#define	SCINTILLAQT_H


#include <QClipboard>
#include <QObject>

#include <Xsci/xsciglobal.h>

// These are needed because Scintilla class header files don't manage their own
// dependencies properly.
#include <algorithm>
#include <assert.h>
#include <ctype.h>
#include <memory>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include "ILexer.h"
#include "ILoader.h"
#include "Platform.h"
#include "Scintilla.h"
#include "SplitVector.h"
#include "Partitioning.h"
#include "Position.h"
#include "UniqueString.h"
#include "CellBuffer.h"
#include "CharClassify.h"
#include "RunStyles.h"
#include "CaseFolder.h"
#include "Decoration.h"
#include "Document.h"
#include "Style.h"
#include "XPM.h"
#include "LineMarker.h"
#include "Indicator.h"
#include "ViewStyle.h"
#include "KeyMap.h"
#include "ContractionState.h"
#include "Selection.h"
#include "PositionCache.h"
#include "EditModel.h"
#include "MarginView.h"
#include "EditView.h"
#include "Editor.h"
#include "AutoComplete.h"
#include "CallTip.h"
#include "LexAccessor.h"
#include "Accessor.h"

#include "ScintillaBase.h"


QT_BEGIN_NAMESPACE
class QMimeData;
class QPaintEvent;
QT_END_NAMESPACE

class XsciScintillaBase;
class XsciSciCallTip;
class XsciSciPopup;


// This is an internal class but it is referenced by a public class so it has
// to have a Xsci prefix rather than being put in the Scintilla namespace.
// (However the reason for avoiding this no longer applies.)
class XsciScintillaQt : public QObject, public Scintilla::ScintillaBase
{
    Q_OBJECT

	friend class XsciScintillaBase;
	friend class XsciSciCallTip;
	friend class XsciSciPopup;

public:
	XsciScintillaQt(XsciScintillaBase *qsb_);
	virtual ~XsciScintillaQt();

	virtual sptr_t WndProc(unsigned int iMessage, uptr_t wParam,
            sptr_t lParam);

protected:
    void timerEvent(QTimerEvent *e);

private slots:
    void onIdle();
    void onSelectionChanged();

private:
	void Initialise();
	void Finalise();
    bool SetIdle(bool on);
	void StartDrag();
	sptr_t DefWndProc(unsigned int, uptr_t, sptr_t);
	void SetMouseCapture(bool on);
	bool HaveMouseCapture();
	void SetVerticalScrollPos();
	void SetHorizontalScrollPos();
	bool ModifyScrollBars(Sci::Line nMax, Sci::Line nPage);
	void ReconfigureScrollBars();
	void NotifyChange();
	void NotifyParent(SCNotification scn);
	void CopyToClipboard(const Scintilla::SelectionText &selectedText);
	void Copy();
	void Paste();
	void CreateCallTipWindow(Scintilla::PRectangle rc);
	void AddToPopUp(const char *label, int cmd = 0, bool enabled = true);
	void ClaimSelection();
	void UnclaimSelection();
	static sptr_t DirectFunction(XsciScintillaQt *sci, unsigned int iMessage,
            uptr_t wParam,sptr_t lParam);

	QMimeData *mimeSelection(const Scintilla::SelectionText &text) const;
	void paintEvent(QPaintEvent *e);
    void pasteFromClipboard(QClipboard::Mode mode);

    // tickPlatform is the last of the TickReason members.
    int timers[tickPlatform + 1];
    bool FineTickerAvailable();
    void FineTickerCancel(TickReason reason);
    bool FineTickerRunning(TickReason reason);
    void FineTickerStart(TickReason reason, int ms, int tolerance);

    int vMax, hMax, vPage, hPage;
    bool capturedMouse;
    XsciScintillaBase *qsb;
};

#endif
