#!/bin/sh -e
gtkdocize || exit 1
${AUTORECONF:-autoreconf} -fiv
