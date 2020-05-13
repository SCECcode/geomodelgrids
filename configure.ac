# ----------------------------------------------------------------------
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ----------------------------------------------------------------------

AC_PREREQ(2.59)
AC_INIT([geomodelgrids], [0.1.0], [baagaard@usgs.gov])
AC_CONFIG_HEADER([portinfo])
AC_CONFIG_AUX_DIR([./aux-config])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign subdir-objects tar-pax])

# ----------------------------------------------------------------------
# Fortran
AC_ARG_ENABLE([fortran],
	[  --enable-fortran        Enable building Fortran bindings [[default=no]]],
	[enable_fortran=yes],
	[enable_fortran=no])

# TESTING
AC_ARG_ENABLE([testing],
	[  --enable-testing        Enable Python and C++ (requires cppunit) unit testing [[default=no]]],
	[if test "$enableval" = yes ; then enable_testing=yes; else enable_testing=no; fi],
	[enable_testing=no])

# TEST COVERAGE w/locv and python-coverage
AC_ARG_ENABLE([test-coverage],
	[  --enable-test-coverage  Enable test coverage with lcov and python-coverage [[default=no]]],
	[if test "$enableval" = yes ; then enable_test_coverage=yes; else enable_test_coverage=no; fi],
	[enable_test_coverage=no])
AC_ARG_WITH([python-coverage],
    [AC_HELP_STRING([--with-python-coverage],
        [set executable for python-coverage @<:@default=coverage2@:>@])],
	[python_coverage=$withval],
	[python_coverage="coverage2"])
AC_SUBST(python_coverage)

# SPATIAL w/spatialdata extension
AC_ARG_ENABLE([spatial],
	[  --enable-spatial        Enable use of spatialdata package (requires spatialdata) [[default=no]]],
	[enable_spatial=yes],
	[enable_spatial=no])


# ----------------------------------------------------------------------
# Dependency locations
# ----------------------------------------------------------------------
# CPPUNIT
AC_ARG_WITH([cppunit-incdir],
    [AC_HELP_STRING([--with-cppunit-incdir],
        [location of cppunit header files @<:@default=no@:>@])],
	[with_cppunit_incdir=$withval],
	[with_cppunit_incdir=no])
AC_SUBST(with_cppunit_incdir)
AC_ARG_WITH([cppunit-libdir],
    [AC_HELP_STRING([--with-cppunit-libdir],
        [location of cppunit library @<:@default=no@:>@])],
	[with_cppunit_libdir=$withval],
	[with_cppunit_libdir=no])
AC_SUBST(with_cppunit_libdir)

# PROJ
AC_ARG_WITH([proj-incdir],
    [AC_HELP_STRING([--with-proj-incdir],
        [location of proj header files @<:@default=no@:>@])],
	[with_proj_incdir=$withval],
	[with_proj_incdir=no])
AC_SUBST(with_proj_incdir)
AC_ARG_WITH([proj-libdir],
    [AC_HELP_STRING([--with-proj-libdir],
        [location of proj library @<:@default=no@:>@])],
	[with_proj_libdir=$withval],
	[with_proj_libdir=no])
AC_SUBST(with_proj_libdir)

# HDF5
AC_ARG_WITH([hdf5-incdir],
    [AC_HELP_STRING([--with-hdf5-incdir],
        [location of hdf5 header files @<:@default=no@:>@])],
	[with_hdf5_incdir=$withval],
	[with_hdf5_incdir=no])
AC_SUBST(with_hdf5_incdir)
AC_ARG_WITH([hdf5-libdir],
    [AC_HELP_STRING([--with-hdf5-libdir],
        [location of hdf5 library @<:@default=no@:>@])],
	[with_hdf5_libdir=$withval],
	[with_hdf5_libdir=no])
AC_SUBST(with_hdf5_libdir)

# ----------------------------------------------------------------------
# BASIC TOOLS
# ----------------------------------------------------------------------
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_INSTALL

AC_PROG_LIBTOOL
if test "$allow_undefined_flag" = unsupported; then
    # See issue119.
    AM_LDFLAGS="-no-undefined $AM_LDFLAGS"
fi
AM_CONDITIONAL([NO_UNDEFINED], [test "$allow_undefined_flag" = unsupported])
AC_SUBST(AM_LDFLAGS)


# ----------------------------------------------------------------------
# CHECK FOR DEPENDENCIES
# ----------------------------------------------------------------------
# PROJ
if test "$with_proj_incdir" != no; then
  PROJ_INCLUDES="-I$with_proj_incdir"
fi
if test "$with_proj_libdir" != no; then
  PROJ_LDFLAGS="-L$with_proj_libdir"
fi
CIT_PROJ6_HEADER
CIT_PROJ6_LIB
AC_SUBST(PROJ_INCLUDES)
AC_SUBST(PROJ_LDFLAGS)

if test "$with_hdf5_incdir" != no; then
  HDF5_INCLUDES="-I$with_hdf5_incdir"
fi
if test "$with_hdf5_libdir" != no; then
  HDF5_LDFLAGS="-L$with_hdf5_libdir"
fi
CIT_HDF5_HEADER
CIT_HDF5_LIB
AC_SUBST(HDF5_INCLUDES)
AC_SUBST(HDF5_LDFLAGS)

# FORTRAN BINDINGS
AM_CONDITIONAL([ENABLE_FORTRAN], [test "$enable_fortran" = yes])
if test "$enable_fortran" = "yes" ; then
  AC_PROG_FC
  F77 = FC
  AC_FC_WRAPPERS
  AC_FC_LIBRARY_LDFLAGS
fi

# CPPUNIT
AM_CONDITIONAL([ENABLE_TESTING], [test "$enable_testing" = yes])
if test "$enable_testing" = "yes" ; then
  # Require C++-11
  AX_CXX_COMPILE_STDCXX(11)

  if test "$with_cppunit_incdir" != no; then
    CPPUNIT_INCLUDES="-I$with_cppunit_incdir"
  fi
  if test "$with_cppunit_libdir" != no; then
    CPPUNIT_LDFLAGS="-L$with_cppunit_libdir"
  fi
  CIT_CPPUNIT_HEADER
  CIT_CPPUNIT_LIB
fi
AC_SUBST(CPPUNIT_INCLUDES)
AC_SUBST(CPPUNIT_LDFLAGS)

# SPATIALDATA
AM_CONDITIONAL([ENABLE_SPATIAL], [test "$enable_spatial" = yes])
if test "$enable_spatial" = "yes" ; then
  AC_LANG(C++)
  AC_CHECK_HEADER([spatialdata/spatialdb/SpatialDB.hh], [], [
    AC_MSG_ERROR([SpatialDB header not found; try CPPFLAGS="-I<Spatialdata include dir>"])
])
  AC_MSG_CHECKING([for spatialdb::SimpleDB in -lspatialdata])
  AC_REQUIRE_CPP
  AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([[#include <spatialdata/spatialdb/SpatialDB.hh>]
                      [#include <spatialdata/spatialdb/SimpleDB.hh>]],
                     [[spatialdata::spatialdb::SimpleDB db;]])],
    [AC_MSG_RESULT(yes)],
    [AC_MSG_RESULT(no)
     AC_MSG_ERROR([Spatialdata library not found; try LDFLAGS="-L<Spatialdata lib dir>"])
  ])
  if test "x$SPATIALDATA_SWIG_DIR" != "x" ; then
    AC_CHECK_FILE([$SPATIALDATA_SWIG_DIR/spatialdb/SpatialDBObj.i], [], [
      AC_MSG_ERROR([Spatialdata SpatialDBObj.i SWIG interface file not found])])
    AC_SUBST([SWIG_FLAGS], ["-I$SPATIALDATA_SWIG_DIR $SWIG_FLAGS"])
  else
    AC_CHECK_FILE([spatialdata/SpatialDBObj.i], [], [
      AC_MSG_ERROR([Spatialdata SpatialDBObj.i SWIG interface file not found; Try setting SPATIALDATA_SWIG_DIR=<directory containing spatialdb/SpatialDBObj.i>])])
  fi

  # PYTHON
  AM_PATH_PYTHON([3.6])
  CIT_PYTHON_SYSCONFIG

  # SWIG
  CIT_NUMPY_PYTHON_MODULE
  CIT_NUMPY_INCDIR
  AC_PROG_SWIG([2.0.4], [3.0.2])

fi

# TEST COVERAGE
AM_CONDITIONAL([ENABLE_TEST_COVERAGE], [test "$enable_test_coverage" = yes])
if test "$enable_test_coverage" = "yes" ; then
  AC_PATH_PROG(LCOV, lcov)
  if test -z "$LCOV"; then
    AC_MSG_FAILURE([cannot find 'lcov' program for C++ test coverage.])
  fi
  AC_PATH_PROG(PYTHON_COVERAGE, $python_coverage)
  if test -z "$PYTHON_COVERAGE"; then
    AC_MSG_FAILURE([cannot find '$python_coverage' program for Python test coverage.])
  fi
fi

# VERSION
CIG_PKG_GIT(GEOMODELGRIDS)
AC_DEFINE_UNQUOTED([GEOMODELGRIDS_VERSION], ["$PACKAGE_VERSION"], [Define geomodelgrids version])



# ----------------------------------------------------------------------
AC_CONFIG_FILES([Makefile
	libsrc/Makefile
	libsrc/geomodelgrids/Makefile
	libsrc/geomodelgrids/apps/Makefile
	libsrc/geomodelgrids/serial/Makefile
	libsrc/geomodelgrids/utils/Makefile
	bin/Makefile
	geomodelgrids/Makefile
	tests/Makefile
	tests/data/Makefile
	tests/libtests/Makefile
	tests/libtests/utils/Makefile
	tests/libtests/serial/Makefile
	tests/libtests/apps/Makefile
	docs/Makefile
	models/Makefile
	])

AC_OUTPUT


# End of file