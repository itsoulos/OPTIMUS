#ifndef SODE1_GLOBAL_H
#define SODE1_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SODE1_LIBRARY)
#  define SODE1_EXPORT Q_DECL_EXPORT
#else
#  define SODE1_EXPORT Q_DECL_IMPORT
#endif

#endif // SODE1_GLOBAL_H
