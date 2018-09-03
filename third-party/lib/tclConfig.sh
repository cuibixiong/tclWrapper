# tclConfig.sh --
#
# This shell script (for sh) is generated automatically by Tcl's
# configure script.  It will create shell variables for most of
# the configuration options discovered by the configure script.
# This script is intended to be included by the configure scripts
# for Tcl extensions so that they don't have to figure this all
# out for themselves.
#
# The information in this file is specific to a single platform.

# Tcl's version number.
TCL_VERSION='8.6'
TCL_MAJOR_VERSION='8'
TCL_MINOR_VERSION='6'
TCL_PATCH_LEVEL='.8'

# C compiler to use for compilation.
TCL_CC='gcc'

# -D flags for use with the C compiler.
TCL_DEFS='-DHAVE_TCL_CONFIG_H  -imacros tclConfig.h'

# TCL_DBGX used to be used to distinguish debug vs. non-debug builds.
# This was a righteous pain so the core doesn't do that any more.
TCL_DBGX=

# Default flags used in an optimized and debuggable build, respectively.
TCL_CFLAGS_DEBUG='-g'
TCL_CFLAGS_OPTIMIZE='-Os'

# Default linker flags used in an optimized and debuggable build, respectively.
TCL_LDFLAGS_DEBUG=''
TCL_LDFLAGS_OPTIMIZE=''

# Flag, 1: we built a shared lib, 0 we didn't
TCL_SHARED_BUILD=1

# The name of the Tcl library (may be either a .a file or a shared library):
TCL_LIB_FILE='libtcl8.6.dylib'

# Additional libraries to use when linking Tcl.
TCL_LIBS=' -lz  -lpthread -framework CoreFoundation '

# Top-level directory in which Tcl's platform-independent files are
# installed.
TCL_PREFIX='/Users/cuibixiong/works/Github/tclWrapper/third-party'

# Top-level directory in which Tcl's platform-specific files (e.g.
# executables) are installed.
TCL_EXEC_PREFIX='/Users/cuibixiong/works/Github/tclWrapper/third-party'

# Flags to pass to cc when compiling the components of a shared library:
TCL_SHLIB_CFLAGS='-fno-common'

# Flags to pass to cc to get warning messages
TCL_CFLAGS_WARNING='-Wall'

# Extra flags to pass to cc:
TCL_EXTRA_CFLAGS='-pipe   '

# Base command to use for combining object files into a shared library:
TCL_SHLIB_LD='${CC} -dynamiclib ${CFLAGS} ${LDFLAGS} -Wl,-single_module'

# Base command to use for combining object files into a static library:
TCL_STLIB_LD='${AR} cr'

# Either '$LIBS' (if dependent libraries should be included when linking
# shared libraries) or an empty string.  See Tcl's configure.in for more
# explanation.
TCL_SHLIB_LD_LIBS='${LIBS}'

# Suffix to use for the name of a shared library.
TCL_SHLIB_SUFFIX='.dylib'

# Library file(s) to include in tclsh and other base applications
# in order to provide facilities needed by DLOBJ above.
TCL_DL_LIBS=''

# Flags to pass to the compiler when linking object files into
# an executable tclsh or tcltest binary.
TCL_LD_FLAGS=' -prebind -headerpad_max_install_names -Wl,-search_paths_first '

# Flags to pass to cc/ld, such as "-R /usr/local/tcl/lib", that tell the
# run-time dynamic linker where to look for shared libraries such as
# libtcl.so.  Used when linking applications.  Only works if there
# is a variable "LIB_RUNTIME_DIR" defined in the Makefile.
TCL_CC_SEARCH_FLAGS=''
TCL_LD_SEARCH_FLAGS=''

# Additional object files linked with Tcl to provide compatibility
# with standard facilities from ANSI C or POSIX.
TCL_COMPAT_OBJS=''

# Name of the ranlib program to use.
TCL_RANLIB='ranlib'

# -l flag to pass to the linker to pick up the Tcl library
TCL_LIB_FLAG='-ltcl8.6'

# String to pass to linker to pick up the Tcl library from its
# build directory.
TCL_BUILD_LIB_SPEC='-L/Users/cuibixiong/works/Github/tclWrapper/third-party/build -ltcl8.6'

# String to pass to linker to pick up the Tcl library from its
# installed directory.
TCL_LIB_SPEC='-L/Users/cuibixiong/works/Github/tclWrapper/third-party/lib -ltcl8.6'

# String to pass to the compiler so that an extension can
# find installed Tcl headers.
TCL_INCLUDE_SPEC='-I/Users/cuibixiong/works/Github/tclWrapper/third-party/include'

# Indicates whether a version numbers should be used in -l switches
# ("ok" means it's safe to use switches like -ltcl7.5;  "nodots" means
# use switches like -ltcl75).  SunOS and FreeBSD require "nodots", for
# example.
TCL_LIB_VERSIONS_OK='ok'

# String that can be evaluated to generate the part of a shared library
# name that comes after the "libxxx" (includes version number, if any,
# extension, and anything else needed).  May depend on the variables
# VERSION and SHLIB_SUFFIX.  On most UNIX systems this is
# ${VERSION}${SHLIB_SUFFIX}.
TCL_SHARED_LIB_SUFFIX='${VERSION}.dylib'

# String that can be evaluated to generate the part of an unshared library
# name that comes after the "libxxx" (includes version number, if any,
# extension, and anything else needed).  May depend on the variable
# VERSION.  On most UNIX systems this is ${VERSION}.a.
TCL_UNSHARED_LIB_SUFFIX='${VERSION}.a'

# Location of the top-level source directory from which Tcl was built.
# This is the directory that contains a README file as well as
# subdirectories such as generic, unix, etc.  If Tcl was compiled in a
# different place than the directory containing the source files, this
# points to the location of the sources, not the location where Tcl was
# compiled.
TCL_SRC_DIR='/Users/cuibixiong/works/Github/tclWrapper/third-party/tcl8.6.8'

# List of standard directories in which to look for packages during
# "package require" commands.  Contains the "prefix" directory plus also
# the "exec_prefix" directory, if it is different.
TCL_PACKAGE_PATH='/Users/cuibixiong/works/Github/tclWrapper/third-party/lib '

# Tcl supports stub.
TCL_SUPPORTS_STUBS=1

# The name of the Tcl stub library (.a):
TCL_STUB_LIB_FILE='libtclstub8.6.a'

# -l flag to pass to the linker to pick up the Tcl stub library
TCL_STUB_LIB_FLAG='-ltclstub8.6'

# String to pass to linker to pick up the Tcl stub library from its
# build directory.
TCL_BUILD_STUB_LIB_SPEC='-L/Users/cuibixiong/works/Github/tclWrapper/third-party/build -ltclstub8.6'

# String to pass to linker to pick up the Tcl stub library from its
# installed directory.
TCL_STUB_LIB_SPEC='-L/Users/cuibixiong/works/Github/tclWrapper/third-party/lib -ltclstub8.6'

# Path to the Tcl stub library in the build directory.
TCL_BUILD_STUB_LIB_PATH='/Users/cuibixiong/works/Github/tclWrapper/third-party/build/libtclstub8.6.a'

# Path to the Tcl stub library in the install directory.
TCL_STUB_LIB_PATH='/Users/cuibixiong/works/Github/tclWrapper/third-party/lib/libtclstub8.6.a'

# Flag, 1: we built Tcl with threads enabled, 0 we didn't
TCL_THREADS=1
