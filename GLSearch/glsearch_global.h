#ifndef GLSEARCH_GLOBAL_H
#define GLSEARCH_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GLSEARCH_LIBRARY)
#  define GLSEARCH_EXPORT Q_DECL_EXPORT
#else
#  define GLSEARCH_EXPORT Q_DECL_IMPORT
#endif

#endif // GLSEARCH_GLOBAL_H
