AC_INIT([libfc14audiodecoder], [1.0.3])
AC_CONFIG_SRCDIR([src/FC.cpp])
AM_INIT_AUTOMAKE([foreign])
AM_CONFIG_HEADER([src/Config.h])
AC_CONFIG_MACRO_DIR([m4])

LIBVERSION=1:0:0
AC_SUBST(LIBVERSION)dnl libtool-style version

AC_CANONICAL_HOST
cpu=""
case $host in
	dnl Deliberately ignoring i486 and older.
    i586-*)
   	  cpu="x86"
	;;
    i686-*)
   	  cpu="x86"
	;;
    x86_64-*)
   	  cpu="x86"
	;;
esac

AC_PROG_CC
AC_LANG_C

AC_C_BIGENDIAN

AC_CHECK_SIZEOF(char)
AC_CHECK_SIZEOF(short int)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long int)
if test $ac_cv_sizeof_long_int -lt 4; then
  AC_MSG_ERROR([platform unsupported: sizeof(long int) < 4]);
fi

AC_HEADER_STDC

AC_PATH_PROG(RM,rm,rm)

AC_PROG_CXX
AC_LANG_CPLUSPLUS

AC_DEFUN([FC_CHECK_NOTHROW],
[
    AC_MSG_CHECKING(whether nothrow allocator is available)
    AC_CACHE_VAL(fc_cv_have_nothrow,
    [
        AC_TRY_COMPILE(
            [#include <new>],
		    [char* buf = new(std::nothrow) char[1024];],
		    [fc_cv_have_nothrow=yes],
		    [fc_cv_have_nothrow=no]
	    )
    ])
    AC_MSG_RESULT($fc_cv_have_nothrow)
    if test "$fc_cv_have_nothrow" = yes; then
	AC_DEFINE([FC_HAVE_NOTHROW], 1, [Define if ``nothrow allocator'' is available.])
    fi
])
FC_CHECK_NOTHROW

AM_DISABLE_STATIC
AM_PROG_LIBTOOL

AC_CHECK_HEADERS(string iostream)
if test "$ac_cv_header_string" = no || test "$ac_cv_header_iostream" = no; then
    AC_MSG_ERROR(
[Standard C++ library header files not found!]
     );
fi

AC_SUBST(CXXFLAGS)

AH_TEMPLATE([OPTIMIZE_ENDIAN_ACCESS],
[Define this to make little/big endian machines access little/big endian
values in memory structures or arrays directly, disregarding alignment])
AC_ARG_ENABLE(optendian,[  --enable-optendian      whether to optimize little-/big-endian conversion])
dnl Also support--disable-optendian and --enable-optendian=no 
if test "$enable_optendian" = yes; then
    AC_DEFINE([OPTIMIZE_ENDIAN_ACCESS])
    AC_MSG_WARN([OPTIMIZE_ENDIAN_ACCESS enabled])
elif test "$enable_optendian" = no; then
	 AC_MSG_WARN([OPTIMIZE_ENDIAN_ACCESS disabled])
elif test "x$cpu" == "xx86"; then
    AC_DEFINE([OPTIMIZE_ENDIAN_ACCESS])
    AC_MSG_NOTICE([OPTIMIZE_ENDIAN_ACCESS enabled automatically for $host])
fi

AC_OUTPUT(Makefile src/Makefile)
