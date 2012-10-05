AM_LDFLAGS = \
	-Wl,--as-needed

data_DATA =

bin_PROGRAMS = \
	alsa-loop

sbin_SCRIPTS =

alsa_loop_SOURCES = \
	loop.c

alsa_loop_CPPFLAGS = \
	$(AM_CPPFLAGS) $(ALSA_CFLAGS) -Wall

alsa_loop_LDADD = \
	$(ALSA_LIBS) -lm

man_MANS = 

EXTRA_DIST = 

distclean-local:
	rm -rf autom4te.cache
