#!/bin/bash

 ./configure --with-xerces=$LOCALINST_DIR --with-xerces-inc=$LOCALINST_DIR/include --with-xerces-lib=$LOCALINST_DIR/lib --prefix=$LOCALINST_DIR CXXFLAGS=-I$LOCALINST_DIR/include/ LDFLAGS=-L$LOCALINST_DIR/lib
