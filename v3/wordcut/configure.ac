dnl Process this file with autoconf to produce a configure script.
AC_INIT(src/wcdict.c)
AM_INIT_AUTOMAKE(wordcut,0.1.3)
#AM_INIT_AUTOMAKE(wordcut,0.1.2)
AC_SUBST(VERSION)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AM_PROG_LIBTOOL

#AC_PKG_FIND(GLIB2,glib-2.0)
#AC_SUBST(GLIB2_CFLAGS)
#AC_SUBST(GLIB2_LIBS)

dnl Checks for header files.
#AC_STDC_HEADERS
AC_HAVE_HEADERS(string.h)

dnl Checks for typedefs, structures, and compiler characteristics.
#AC_C_CONST

dnl Checks for library functions.
#AC_FUNC_ALLOCA
AC_OUTPUT(	
	Makefile 
	test/Makefile
	test/path.h
	example/Makefile
	data/Makefile
	src/Makefile 
	src/encoder/Makefile 
	src/util/Makefile 
	include/Makefile 
	include/wordcut/Makefile 
	include/wordcut/wcpath.h
	wordcut.pc
	wordcut.spec
)
