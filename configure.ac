AC_INIT([usb-detour-client],
	[001],
	[usb-detour@zonque.org])
AC_PREREQ(2.60)
AM_INIT_AUTOMAKE([check-news foreign 1.9 dist-bzip2])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_PREFIX_DEFAULT([/usr])

AC_USE_SYSTEM_EXTENSIONS
AC_SYS_LARGEFILE

PKG_CHECK_MODULES(ALSA,	[ alsa >= 1.0.22 ])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile
])

AC_OUTPUT
AC_MSG_RESULT([
	usb-detour $VERSION
	=============

	prefix:                 ${prefix}
	datarootdir:            ${datarootdir}
	datadir:                ${datadir}
	mandir:                 ${mandir}

	compiler:               ${CC}
	cflags:                 ${CFLAGS}
	ldflags:                ${LDFLAGS}
])
