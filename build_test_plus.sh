#!/usr/bin/env bash
mkdir -p bin
gcc const.c graph_node.c plus.c print.c test_plus.c -o \
    bin/test_plus -g
