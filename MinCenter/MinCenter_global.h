#ifndef MINCENTER_GLOBAL_H
#define MINCENTER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MINCENTER_LIBRARY)
#  define MINCENTER_EXPORT Q_DECL_EXPORT
#else
#  define MINCENTER_EXPORT Q_DECL_IMPORT
#endif

#endif // MINCENTER_GLOBAL_H
