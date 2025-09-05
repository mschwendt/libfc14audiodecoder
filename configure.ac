AC_INIT([libfc14audiodecoder], [2.0.0])
AC_CONFIG_SRCDIR([src/FC.cpp])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_HEADERS([src/Config.h])
AC_CONFIG_MACRO_DIR([m4])

LIBVERSION=1:0:0
AC_SUBST(LIBVERSION)dnl libtool-style version

AC_CANONICAL_HOST

AC_PROG_CXX
AC_PROG_AR
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PATH_PROG(RM,rm,rm)

AC_LANG([C])

AC_C_INLINE
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT8_T
AC_TYPE_UINT8_T
AC_TYPE_INT16_T
AC_TYPE_UINT16_T
AC_TYPE_INT32_T
AC_TYPE_UINT32_T

AC_LANG([C++])

AC_DEFUN([FC_CHECK_NOTHROW],
[
    AC_MSG_CHECKING(whether nothrow allocator is available)
    AC_CACHE_VAL(fc_cv_have_nothrow,
    [
        AC_COMPILE_IFELSE(
            [AC_LANG_PROGRAM([[#include <new>]],
		    [[char* buf = new(std::nothrow) char[1024];]])],
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

AC_SUBST(CXXFLAGS)

LT_INIT([])

AC_CONFIG_FILES([Makefile libfc14audiodecoder.pc src/Makefile])
AC_OUTPUT
