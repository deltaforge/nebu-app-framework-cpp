# Define basic library information
AC_PREREQ(2.61)
AC_INIT([libnebu-app-framework], [0.1.0], [])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.11 foreign subdir-objects])
LT_PREREQ([2.2])

# Check for programs
OLD_CXXFLAGS="$CXXFLAGS"
AC_PROG_CC
AC_PROG_CXX
CXXFLAGS="$OLD_CXXFLAGS"
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_LIBTOOL

# Check for libraries
PKG_CHECK_MODULES([TINYXML2], [tinyxml2])
AC_SUBST([TINYXML2_CFLAGS])
AC_SUBST([TINYXML2_LIBS])
PKG_CHECK_MODULES([NEBU_COMMON], [libnebu-common >= 0.1.0])
AC_SUBST([NEBU_COMMON_CFLAGS])
AC_SUBST([NEBU_COMMON_LIBS])
PKG_CHECK_MODULES([LOG4CXX], [liblog4cxx])
AC_SUBST([LOG4CXX_CFLAGS])
AC_SUBST([LOG4CXX_LIBS])

# Define test configuration option
AC_ARG_ENABLE([tests],
	[	--enable-tests   Turn on test and coverage flags],
	[case "${enableval}" in
	  yes) tests=true ;;
	  no)  tests=false ;;
	  *) AC_MSG_ERROR([bad value ${enableval} for --enable-tests]) ;;
	esac],[tests=false])
AM_CONDITIONAL([TESTS_ENABLED], [test x$tests = xtrue])

# Define Makefiles
AC_CONFIG_FILES([
	Makefile
	include/Makefile
	src/Makefile
	doc/doxygen.cfg
	doc/Makefile
	test/Makefile
	libnebu-app-framework.pc
])

# Generate 'configure' 
AC_OUTPUT
