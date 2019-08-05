dnl Process this file with autoconf to produce a configure script.
AC_INIT(options, git)
AC_CONFIG_SRCDIR(example.cpp)
AC_CANONICAL_SYSTEM

dnl create an config-header               
AC_CONFIG_HEADER(config.h)

dnl enable automake-support
AM_INIT_AUTOMAKE([dist-zip tar-ustar])

AC_LANG([C++])

# avoiding warnings on ubuntu 16.04, there seem to be two variants of the ARFLAGS, one for automake, one for libttol
ARFLAGS=cr
AC_SUBST(ARFLAGS)
AR_FLAGS=cr
AC_SUBST(AR_FLAGS)

dnl the debug check has to be made before the program checks because it modifies
dnl CXXFLAGS.

AC_ARG_ENABLE([debug], [AS_HELP_STRING([--enable-debug],[enable debugging code [default=no].])])
if test x$enable_debug = xyes; then
    AC_DEFINE(_DEBUG, 1, [Define to 1 in order to enable sumo debugging code.])
    CXXFLAGS="-std=c++11 -g -pthread $CXXFLAGS"
    ac_enabled="$ac_enabled Debug"
    program_transform_name="s,\$\$,D,; $program_transform_name"
else
    CXXFLAGS="-std=c++11 -O2 -DNDEBUG $CXXFLAGS"
    ac_disabled="$ac_disabled Debug"
fi

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

dnl enable libtool-support
dnl gives a deprecated warning for RANLIB with newer libtools
AC_PROG_RANLIB
AM_PROG_LIBTOOL


AM_CONDITIONAL([WITH_VERSION_H], test -d .git)
if test -d .git || test x$PACKAGE_VERSION == xgit -a -f version.h; then
   AC_DEFINE([HAVE_VERSION_H], [1], [Define if auto-generated version.h should be used.])
else
   AC_DEFINE_UNQUOTED(VERSION_STRING, "$PACKAGE_VERSION", [Defined if auto-generated version.h is unavailable.])
fi

dnl ---------------------------------------------
dnl Platform specific stuff
dnl - - - - - - - - - - - - - - - - - - - - - - -
dnl - - - - - - - - - - - - - - - - - - - - - - -
dnl  general flags
AM_CPPFLAGS="-I$PWD/$srcdir/src $AM_CPPFLAGS"
AC_SUBST(AM_CPPFLAGS)
AC_SUBST(AM_CXXFLAGS)
AC_DEFINE_UNQUOTED([HAVE_ENABLED], ["$host$ac_enabled"], [Reporting string for enabled options])


dnl ---------------------------------------------
dnl Checks proposed by autoscan.
dnl Checks for header files.
AC_FUNC_ALLOCA
AC_HEADER_STDC
AC_CHECK_HEADERS([])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM
AC_C_VOLATILE

dnl Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_SELECT_ARGTYPES
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_CHECK_FUNCS([pow])


dnl ---------------------------------------------
dnl add here all your Makefiles. These will be created by configure
AC_CONFIG_FILES([
utils/Makefile
utils/options/Makefile
Makefile])
AC_OUTPUT

dnl ---------------------------------------------
dnl Summary

AC_MSG_RESULT()
AC_MSG_RESULT([Optional features summary])
AC_MSG_RESULT([-------------------------])
AC_MSG_RESULT([Enabled:$ac_enabled])
AC_MSG_RESULT([Disabled:$ac_disabled])
