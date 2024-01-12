#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(FILEREADER_LIB)
#  define FILEREADER_EXPORT Q_DECL_EXPORT
# else
#  define FILEREADER_EXPORT Q_DECL_IMPORT
# endif
#else
# define FILEREADER_EXPORT
#endif
