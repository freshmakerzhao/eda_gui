// Support for building the Scintilla code in the Scintilla namespace using the
// -DSCI_NAMESPACE compiler flag.



#ifndef _SCINAMESPACE_H
#define _SCINAMESPACE_H

#ifdef SCI_NAMESPACE
#define EXSCI_SCI_NAMESPACE(name)    Scintilla::name
#define EXSCI_BEGIN_SCI_NAMESPACE    namespace Scintilla {
#define EXSCI_END_SCI_NAMESPACE      };
#else
#define EXSCI_SCI_NAMESPACE(name)    name
#define EXSCI_BEGIN_SCI_NAMESPACE
#define EXSCI_END_SCI_NAMESPACE
#endif

#endif
