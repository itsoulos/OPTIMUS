#ifndef OPTIMUS_GLOBAL_H
#define OPTIMUS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(OPTIMUS_LIBRARY)
#  define OPTIMUS_EXPORT Q_DECL_EXPORT
#else
#  define OPTIMUS_EXPORT Q_DECL_IMPORT
#endif

#endif // OPTIMUS_GLOBAL_H
