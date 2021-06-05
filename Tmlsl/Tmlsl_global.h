#ifndef TMLSL_GLOBAL_H
#define TMLSL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TMLSL_LIBRARY)
#  define TMLSL_EXPORT Q_DECL_EXPORT
#else
#  define TMLSL_EXPORT Q_DECL_IMPORT
#endif

#endif // TMLSL_GLOBAL_H
