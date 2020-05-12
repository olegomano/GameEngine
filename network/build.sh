#!/bin/bash

SRC_DIR=engproto
DST_DIR=message
UI_DIR=ui
touch $DST_DIR/__init__.py
touch $DST_DIR/$SRC_DIR/__init__.py
protoc -I=./ --python_out=$DST_DIR $SRC_DIR/*.proto

for f in $UI_DIR/*.ui;do
  name=${f%.*}
  pyuic5 $f -o "$name"_ui.py
done
