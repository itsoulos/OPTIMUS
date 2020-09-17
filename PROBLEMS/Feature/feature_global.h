#ifndef FEATURE_GLOBAL_H
#define FEATURE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FEATURE_LIBRARY)
#  define FEATURESHARED_EXPORT Q_DECL_EXPORT
#else
#  define FEATURESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FEATURE_GLOBAL_H
