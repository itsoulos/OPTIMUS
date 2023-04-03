#ifndef PBRENT_GLOBAL_H
#define PBRENT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PBRENT_LIBRARY)
#  define PBRENT_EXPORT Q_DECL_EXPORT
#else
#  define PBRENT_EXPORT Q_DECL_IMPORT
#endif

#endif // PBRENT_GLOBAL_H
