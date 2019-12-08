#!/bin/bash
sudo perf_4.19 record -a -g -F 5000 --call-graph fp ${@:1:1}
sudo perf_4.19 report -G --call-graph
sudo perf_4.19 stat -d ${@:1:1}