#ifndef RBFTEST_GLOBAL_H
#define RBFTEST_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RBFTEST_LIBRARY)
#  define RBFTEST_EXPORT Q_DECL_EXPORT
#else
#  define RBFTEST_EXPORT Q_DECL_IMPORT
#endif

#endif // RBFTEST_GLOBAL_H
