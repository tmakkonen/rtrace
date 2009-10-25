AC_INIT([rtrace], [0.1], [tuomo.makkonen@iki.fi])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_MAKE_SET
AC_LANG(C++)
CFLAGS="-O2"
CXXFLAGS="-O2"

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_INT64_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_CHECK_FUNCS([sqrt])

# Boost check
CPPFLAGS="${CPPFLAGS} -I/opt/local/include"

AC_CHECK_HEADERS([boost/spirit.hpp], [], [AC_MSG_ERROR([Please install boost libraries.])])

AC_CONFIG_FILES([json_spirit/Makefile common/Makefile app/Makefile shapes/Makefile])
AC_OUTPUT([Makefile])
