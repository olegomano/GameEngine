#!/bin/bash

SRC_DIR=engproto
DST_DIR=message
touch $DST_DIR/__init__.py
touch $DST_DIR/$SRC_DIR/__init__.py
protoc -I=./ --python_out=$DST_DIR $SRC_DIR/*.proto
