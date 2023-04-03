#ifndef IPSO_GLOBAL_H
#define IPSO_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(IPSO_LIBRARY)
#  define IPSO_EXPORT Q_DECL_EXPORT
#else
#  define IPSO_EXPORT Q_DECL_IMPORT
#endif

#endif // IPSO_GLOBAL_H
