#!/bin/sh

rm -Rf build
mkdir build

cd build && cmake -DMYSQL_INC_DIR="/usr/local/include/mysql" .. && make
