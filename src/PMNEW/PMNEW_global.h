#ifndef PMNEW_GLOBAL_H
#define PMNEW_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PMNEW_LIBRARY)
#  define PMNEW_EXPORT Q_DECL_EXPORT
#else
#  define PMNEW_EXPORT Q_DECL_IMPORT
#endif

#endif // PMNEW_GLOBAL_H
