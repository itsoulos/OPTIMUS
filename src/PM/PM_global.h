#ifndef PM_GLOBAL_H
#define PM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PM_LIBRARY)
#  define PM_EXPORT Q_DECL_EXPORT
#else
#  define PM_EXPORT Q_DECL_IMPORT
#endif

#endif // PM_GLOBAL_H
