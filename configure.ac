AC_PREREQ(2.59)
AC_INIT(luksmeta, 9)
AC_CANONICAL_SYSTEM
AC_PROG_CC_C99

AM_INIT_AUTOMAKE([subdir-objects foreign no-dist-gzip dist-bzip2 parallel-tests])
AM_SILENT_RULES([yes])
AM_PROG_CC_C_O

LT_INIT([disable-static])

PKG_PROG_PKG_CONFIG([0.25])
PKG_CHECK_MODULES([cryptsetup], [libcryptsetup >= 1.5.1])

LUKSMETA_CFLAGS="\
-Wall \
-Wextra \
-Werror \
-Wstrict-aliasing \
-Wchar-subscripts \
-Wformat-security \
-Wmissing-declarations \
-Wmissing-prototypes \
-Wnested-externs \
-Wpointer-arith \
-Wshadow \
-Wsign-compare \
-Wstrict-prototypes \
-Wtype-limits \
-Wunused-function \
-Wno-missing-field-initializers \
-Wno-unused-parameter \
"
AC_SUBST([LUKSMETA_CFLAGS])

AC_CHECK_PROGS(A2X, [a2x])

if test "x$A2X" = "x"; then
   AC_MSG_WARN([asciidoc / a2x not found -- man pages will not be generated and installed!])
fi

AM_CONDITIONAL(HAVE_A2X, [test -n "$A2X"])

AC_CONFIG_FILES([luksmeta.pc Makefile])
AC_OUTPUT
