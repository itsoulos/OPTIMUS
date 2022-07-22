#ifndef SEARCH_GLOBAL_H
#define SEARCH_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SEARCH_LIBRARY)
#define SEARCH_EXPORT Q_DECL_EXPORT
#else
#define SEARCH_EXPORT Q_DECL_IMPORT
#endif

#endif // SEARCH_GLOBAL_H
