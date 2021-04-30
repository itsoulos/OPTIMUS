#ifndef GCRS_GLOBAL_H
#define GCRS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GCRS_LIBRARY)
#define GCRS_EXPORT Q_DECL_EXPORT
#else
#define GCRS_EXPORT Q_DECL_IMPORT
#endif

#endif // GCRS_GLOBAL_H
