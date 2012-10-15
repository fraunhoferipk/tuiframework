#!/bin/bash

rm aclocal.m4
rm ax_lib_xerces.m4
rm -rf autom4te.cache
rm config.h
rm config.h.in
rm config.log
rm config.sub
rm configure
rm COPYING
rm depcomp
rm INSTALL
rm install-sh
rm missing
rm stamp-h1
rm config.status
rm ltmain.sh
rm Makefile
rm Makefile.in

find | grep -v .svn | grep -e '\~$' | sort | xargs rm

