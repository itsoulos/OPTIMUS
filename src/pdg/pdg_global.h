#ifndef PDG_GLOBAL_H
#define PDG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PDG_LIBRARY)
#  define PDG_EXPORT Q_DECL_EXPORT
#else
#  define PDG_EXPORT Q_DECL_IMPORT
#endif

#endif // PDG_GLOBAL_H
