#!/bin/bash

aclocal --force

autoheader

automake --add-missing

autoreconf




