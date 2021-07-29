#ifndef PRICE_GLOBAL_H
#define PRICE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PRICE_LIBRARY)
#  define PRICE_EXPORT Q_DECL_EXPORT
#else
#  define PRICE_EXPORT Q_DECL_IMPORT
#endif

#endif // PRICE_GLOBAL_H
