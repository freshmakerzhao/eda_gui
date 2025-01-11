// The definition of the Qt specific subclass of ScintillaBase.



#ifndef SCINTILLAQT_H
#define	SCINTILLAQT_H


#include <QClipboard>
#include <QObject>

#include <Exsci/exsciglobal.h>

#include "SciNamespace.h"

// These are needed because Scintilla class header files don't manage their own
// dependencies properly.
#include <algorithm>
#include <assert.h>
#include <ctype.h>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include "ILexer.h"
#include "Platform.h"
#include "Scintilla.h"
#include "SplitVector.h"
#include "Partitioning.h"
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

class ExsciScintillaBase;
class ExsciSciCallTip;
class ExsciSciPopup;


// This is an internal class but it is referenced by a public class so it has
// to have a Exsci prefix rather than being put in the Scintilla namespace
// which would mean exposing the SCI_NAMESPACE mechanism).
class ExsciScintillaQt : public QObject, public EXSCI_SCI_NAMESPACE(ScintillaBase)
{
    Q_OBJECT

	friend class ExsciScintillaBase;
	friend class ExsciSciCallTip;
	friend class ExsciSciPopup;

public:
	ExsciScintillaQt(ExsciScintillaBase *qsb_);
	virtual ~ExsciScintillaQt();

	virtual sptr_t WndProc(unsigned int iMessage, uptr_t wParam,
            sptr_t lParam);

protected:
    void timerEvent(QTimerEvent *e);

private slots:
    void onIdle();

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
	bool ModifyScrollBars(int nMax, int nPage);
	void ReconfigureScrollBars();
	void NotifyChange();
	void NotifyParent(SCNotification scn);
	void CopyToClipboard(
            const EXSCI_SCI_NAMESPACE(SelectionText) &selectedText);
	void Copy();
	void Paste();
	void CreateCallTipWindow(EXSCI_SCI_NAMESPACE(PRectangle) rc);
	void AddToPopUp(const char *label, int cmd = 0, bool enabled = true);
	void ClaimSelection();
	void UnclaimSelection();
	static sptr_t DirectFunction(ExsciScintillaQt *sci, unsigned int iMessage,
            uptr_t wParam,sptr_t lParam);

	QMimeData *mimeSelection(
            const EXSCI_SCI_NAMESPACE(SelectionText) &text) const;
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
    ExsciScintillaBase *qsb;
};

#endif
