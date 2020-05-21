#!/bin/bash

SRC_DIR=components/engproto
DST_DIR=components/message
UI_DIR=ui
mkdir $DST_DIR
touch $DST_DIR/__init__.py
protoc -I=./$SRC_DIR --python_out=$DST_DIR $SRC_DIR/*.proto

for f in $UI_DIR/*.ui;do
  name=${f%.*}
  pyuic5 $f -o "$name"_ui.py
done
