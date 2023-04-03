#ifndef PSEARCH_GLOBAL_H
#define PSEARCH_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PSEARCH_LIBRARY)
#  define PSEARCH_EXPORT Q_DECL_EXPORT
#else
#  define PSEARCH_EXPORT Q_DECL_IMPORT
#endif

#endif // PSEARCH_GLOBAL_H
