include_HEADERS = smf.h

CLEANFILES =
custom_cflags = -DG_LOG_DOMAIN=\"libsmf\"

if HAVE_INTROSPECTION
-include $(INTROSPECTION_MAKEFILE)
custom_cflags += -DHAVE_INTROSPECTION=1
else
custom_cflags += -DHAVE_INTROSPECTION=0
endif

introspectable_sources = \
    smf.h \
    smf.c \
    smf_decode.c \
    smf_load.c \
    smf_save.c \
    smf_tempo.c

lib_LTLIBRARIES = libsmf.la
libsmf_la_SOURCES = \
    $(introspectable_sources) \
    smf_private.h \
    smf_compat.c
libsmf_la_CFLAGS = \
    $(GLIB_CFLAGS) \
    $(INTROSPECTION_CFLAGS) \
    $(custom_cflags)
libsmf_la_LIBADD = \
    $(GLIB_LIBS) \
    $(INTROSPECTION_LIBS) \
    $(WS2_32_IF_NEEDED)
libsmf_la_LDFLAGS = \
    -no-undefined \
    -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

bin_PROGRAMS = smfsh
smfsh_SOURCES = smfsh.c
smfsh_CFLAGS = \
    $(GLIB_CFLAGS) \
    $(custom_cflags)
smfsh_LDADD = \
    libsmf.la \
    $(GLIB_LIBS) \
    $(READLINE_LIBS) \
    -lm

if HAVE_INTROSPECTION
INTROSPECTION_GIRS = Smf-1.0.gir

Smf-1.0.gir: libsmf.la
Smf_1_0_gir_PACKAGES = gobject-2.0
Smf_1_0_gir_LIBS = libsmf.la
Smf_1_0_gir_INCLUDES = GObject-2.0
Smf_1_0_gir_CFLAGS = $(custom_cflags)
Smf_1_0_gir_FILES = $(introspectable_sources)
Smf_1_0_gir_SCANNERFLAGS = --warn-all

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif
