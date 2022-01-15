#ifndef GENDE_GLOBAL_H
#define GENDE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GENDE_LIBRARY)
#  define GENDE_EXPORT Q_DECL_EXPORT
#else
#  define GENDE_EXPORT Q_DECL_IMPORT
#endif

#endif // GENDE_GLOBAL_H
