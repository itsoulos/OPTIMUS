#ifndef GENPRICE_GLOBAL_H
#define GENPRICE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GENPRICE_LIBRARY)
#  define GENPRICE_EXPORT Q_DECL_EXPORT
#else
#  define GENPRICE_EXPORT Q_DECL_IMPORT
#endif

#endif // GENPRICE_GLOBAL_H
