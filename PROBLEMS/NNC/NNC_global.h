#ifndef NNC_GLOBAL_H
#define NNC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NNC_LIBRARY)
#  define NNC_EXPORT Q_DECL_EXPORT
#else
#  define NNC_EXPORT Q_DECL_IMPORT
#endif

#endif // NNC_GLOBAL_H
