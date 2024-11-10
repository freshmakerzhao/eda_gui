// This module implements the XsciLexerEDIFACT class.
 


#include "Xsci/xscilexeredifact.h"


// The ctor.
XsciLexerEDIFACT::XsciLexerEDIFACT(QObject *parent)
    : XsciLexer(parent)
{
}


// The dtor.
XsciLexerEDIFACT::~XsciLexerEDIFACT()
{
}


// Returns the language name.
const char *XsciLexerEDIFACT::language() const
{
    return "EDIFACT";
}


// Returns the lexer name.
const char *XsciLexerEDIFACT::lexer() const
{
    return "edifact";
}


// Returns the foreground colour of the text for a style.
QColor XsciLexerEDIFACT::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80, 0x80, 0x80);

    case SegmentStart:
        return QColor(0x00, 0x00, 0xcb);

    case SegmentEnd:
        return QColor(0xff, 0x8d, 0xb1);

    case ElementSeparator:
        return QColor(0xff, 0x8d, 0xb1);

    case CompositeSeparator:
        return QColor(0x80, 0x80, 0x00);

    case ReleaseSeparator:
        return QColor(0x5e, 0x5e, 0x5e);

    case UNASegmentHeader:
        return QColor(0x00, 0x80, 0x00);

    case UNHSegmentHeader:
        return QColor(0x2f, 0x8b, 0xbd);

    case BadSegment:
        return QColor(0x80, 0x00, 0x00);
    }

    return XsciLexer::defaultColor(style);
}


// Returns the user name of a style.
QString XsciLexerEDIFACT::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case SegmentStart:
        return tr("Segment start");

    case SegmentEnd:
        return tr("Segment end");

    case ElementSeparator:
        return tr("Element separator");

    case CompositeSeparator:
        return tr("Composite separator");

    case ReleaseSeparator:
        return tr("Release separator");

    case UNASegmentHeader:
        return tr("UNA segment header");

    case UNHSegmentHeader:
        return tr("UNH segment header");

    case BadSegment:
        return tr("Badly formed segment");
    }

    return QString();
}
