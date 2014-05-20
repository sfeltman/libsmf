# We require automake 1.6 at least.
AUTOMAKE_OPTIONS = 1.6

# The name of the module.
DOC_MODULE=libsmf

# The top-level SGML file.
DOC_MAIN_SGML_FILE=$(DOC_MODULE)-docs.xml

# Extra options to supply to gtkdoc-scan
SCAN_OPTIONS=--deprecated-guards="G_DISABLE_DEPRECATED" \
             --ignore-decorators="G_GNUC_WARN_UNUSED_RESULT"

# The directory containing the source code. Relative to $(srcdir)
DOC_SOURCE_DIR=$(top_srcdir)/src

BUILT_HFILES=
HFILE_GLOB=$(top_srcdir)/src/*.h
CFILE_GLOB=$(top_srcdir)/src/*.c

IGNORE_HFILES = \
	smf_private.h

# CFLAGS and LDFLAGS for compiling scan program. Only needed
# if $(DOC_MODULE).types is non-empty.
AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_builddir) \
	-I$(top_builddir)/smf \
	$(GLIB_DEBUG_FLAGS) \
	$(INTROSPECTION_CFLAGS)

GTKDOC_LIBS = \
	$(top_builddir)/src/libsmf.la \
	$(INTROSPECTION_LIBS) \
	$(NULL)

# Extra options to supply to gtkdoc-mkdb
MKDB_OPTIONS = --output-format=xml --sgml-mode --name-space=smf

# Images to copy into HTML directory
HTML_IMAGES =

content_files = \
	smfsh-ref.sgml \
	version.xml

expand_content_files =

extra_files =

# Extra options to supply to gtkdoc-fixref
FIXXREF_OPTIONS=

include $(top_srcdir)/gtk-doc.make

EXTRA_DIST += \
	version.xml.in

man_MANS = \
	libsmf.3 \
	smfsh.1

if ENABLE_MAN

XSLTPROC_FLAGS = \
        --nonet \
        --stringparam man.output.quietly 1 \
        --stringparam funcsynopsis.style ansi \
        --stringparam man.th.extra1.suppress 1 \
        --stringparam man.authors.section.enabled 0 \
        --stringparam man.copyright.section.enabled 0

libsmf.3: docs
		$(AM_V_GEN) $(XSLTPROC) $(XSLTPROC_FLAGS) \
        	http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl \
        	>/dev/null 2>&1 \
        	xml/smf.xml

smfsh.1: smfsh-ref.sgml
		$(AM_V_GEN) $(XSLTPROC) $(XSLTPROC_FLAGS) \
        	http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl \
        	>/dev/null 2>&1 \
        	smfsh-ref.sgml

dist-local-check-mans-enabled:
	if grep "Man generation disabled" $(man_MANS) >/dev/null; then $(RM) $(man_MANS); fi

else

$(man_MANS):
	echo Man generation disabled.  Creating dummy $@.  Configure with --enable-man to enable it.
	@echo Man generation disabled.  Remove this file, configure with --enable-man, and rebuild > $@

dist-local-check-mans-enabled:
	echo "*** --enable-man must be used in order to make dist"
	false

endif

#MAINTAINERCLEANFILES
CLEANFILES = \
	$(man_MANS) \
	$(BUILT_SOURCES)

BUILT_EXTRA_DIST = $(man_MANS)
EXTRA_DIST += \
	$(man_MANS)


if ENABLE_GTK_DOC
TESTS_ENVIRONMENT = \
  DOC_MODULE=$(DOC_MODULE) DOC_MAIN_SGML_FILE=$(DOC_MAIN_SGML_FILE) \
  SRCDIR=$(abs_srcdir) BUILDDIR=$(abs_builddir)
#TESTS = $(GTKDOC_CHECK)
endif

maintainer-clean-local: clean
dist-hook-local: dist-local-check-mans-enabled all-local


