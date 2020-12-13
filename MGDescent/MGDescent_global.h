#ifndef MGDESCENT_GLOBAL_H
#define MGDESCENT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MGDESCENT_LIBRARY)
#  define MGDESCENT_EXPORT Q_DECL_EXPORT
#else
#  define MGDESCENT_EXPORT Q_DECL_IMPORT
#endif

#endif // MGDESCENT_GLOBAL_H
