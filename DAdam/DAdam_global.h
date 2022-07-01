#ifndef DADAM_GLOBAL_H
#define DADAM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DADAM_LIBRARY)
#  define DADAM_EXPORT Q_DECL_EXPORT
#else
#  define DADAM_EXPORT Q_DECL_IMPORT
#endif

#endif // DADAM_GLOBAL_H
