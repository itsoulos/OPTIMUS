#ifndef SMGDESCENT_GLOBAL_H
#define SMGDESCENT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SMGDESCENT_LIBRARY)
#define SMGDESCENT_EXPORT Q_DECL_EXPORT
#else
#define SMGDESCENT_EXPORT Q_DECL_IMPORT
#endif

#endif // SMGDESCENT_GLOBAL_H
