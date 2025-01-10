// This module defines various things common to all of the Scintilla Qt port.



#ifndef EXSCIGLOBAL_H
#define EXSCIGLOBAL_H

#include <qglobal.h>


#define EXSCINTILLA_VERSION      0x020a02
#define EXSCINTILLA_VERSION_STR  "2.10.2"


// Define EXSCINTILLA_MAKE_DLL to create a EXScintilla shared library, or
// define EXSCINTILLA_DLL to link against a EXScintilla shared library, or define
// neither to either build or link against a static EXScintilla library.
#if defined(EXSCINTILLA_DLL)
#define EXSCINTILLA_EXPORT       Q_DECL_IMPORT
#elif defined(EXSCINTILLA_MAKE_DLL)
#define EXSCINTILLA_EXPORT       Q_DECL_EXPORT
#else
#define EXSCINTILLA_EXPORT
#endif


#if !defined(QT_BEGIN_NAMESPACE)
#define QT_BEGIN_NAMESPACE
#define QT_END_NAMESPACE
#endif

#endif
