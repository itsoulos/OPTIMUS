#ifndef MULTISTART_GLOBAL_H
#define MULTISTART_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MULTISTART_LIBRARY)
#  define MULTISTART_EXPORT Q_DECL_EXPORT
#else
#  define MULTISTART_EXPORT Q_DECL_IMPORT
#endif

#endif // MULTISTART_GLOBAL_H
