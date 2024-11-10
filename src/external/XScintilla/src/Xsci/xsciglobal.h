// This module defines various things common to all of the Scintilla Qt port.
 


#ifndef XSCIGLOBAL_H
#define XSCIGLOBAL_H

#include <qglobal.h>


#define XSCINTILLA_VERSION      0x020b03
#define XSCINTILLA_VERSION_STR  "2.11.3"


// Define XSCINTILLA_MAKE_DLL to create a XScintilla shared library, or
// define XSCINTILLA_DLL to link against a XScintilla shared library, or define
// neither to either build or link against a static XScintilla library.
#if defined(XSCINTILLA_DLL)
#define XSCINTILLA_EXPORT       Q_DECL_IMPORT
#elif defined(XSCINTILLA_MAKE_DLL)
#define XSCINTILLA_EXPORT       Q_DECL_EXPORT
#else
#define XSCINTILLA_EXPORT
#endif


#if !defined(QT_BEGIN_NAMESPACE)
#define QT_BEGIN_NAMESPACE
#define QT_END_NAMESPACE
#endif

#endif
