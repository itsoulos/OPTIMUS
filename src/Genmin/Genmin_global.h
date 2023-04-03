#ifndef GENMIN_GLOBAL_H
#define GENMIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GENMIN_LIBRARY)
#  define GENMIN_EXPORT Q_DECL_EXPORT
#else
#  define GENMIN_EXPORT Q_DECL_IMPORT
#endif

#endif // GENMIN_GLOBAL_H
