#ifndef MINFINDER_GLOBAL_H
#define MINFINDER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MINFINDER_LIBRARY)
#  define MINFINDER_EXPORT Q_DECL_EXPORT
#else
#  define MINFINDER_EXPORT Q_DECL_IMPORT
#endif

#endif // MINFINDER_GLOBAL_H
