#!/bin/sh

test -n "$srcdir" || srcdir=`dirname "$0"`
test -n "$srcdir" || srcdir=.

olddir=`pwd`
cd "$srcdir"

ACLOCAL_FLAGS="-I m4 ${ACLOCAL_FLAGS}"

GTKDOCIZE=`which gtkdocize`
if test -z $GTKDOCIZE; then
    echo "*** No GTK-Doc found, documentation won't be generated ***"
    echo 'EXTRA_DIST =' > gtk-doc.make
else
    gtkdocize || exit $?
fi


#${AUTORECONF:-autoreconf} -fiv

AUTORECONF=`which autoreconf`
if test -z $AUTORECONF; then
        echo "*** No autoreconf found, please install it ***"
        exit 1
else
        autoreconf --force --install --verbose || exit $?
fi

cd "$olddir"
test -n "$NOCONFIGURE" || "$srcdir/configure" "$@"
