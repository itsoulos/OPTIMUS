#ifndef BFGS_GLOBAL_H
#define BFGS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BFGS_LIBRARY)
#  define BFGS_EXPORT Q_DECL_EXPORT
#else
#  define BFGS_EXPORT Q_DECL_IMPORT
#endif

#endif // BFGS_GLOBAL_H
