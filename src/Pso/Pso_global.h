#ifndef PSO_GLOBAL_H
#define PSO_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PSO_LIBRARY)
#  define PSO_EXPORT Q_DECL_EXPORT
#else
#  define PSO_EXPORT Q_DECL_IMPORT
#endif

#endif // PSO_GLOBAL_H
