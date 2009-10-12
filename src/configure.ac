#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_INIT([rtrace], [0.1], [tuomo.makkonen@iki.fi])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_MAKE_SET
AC_LANG(C++)

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_INT64_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_CHECK_FUNCS([sqrt])

AC_CONFIG_FILES([json_spirit/Makefile common/Makefile app/Makefile])
AC_OUTPUT([Makefile])
