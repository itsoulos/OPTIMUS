#ifndef DE_GLOBAL_H
#define DE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DE_LIBRARY)
#  define DE_EXPORT Q_DECL_EXPORT
#else
#  define DE_EXPORT Q_DECL_IMPORT
#endif

#endif // DE_GLOBAL_H
