#!/bin/sh

rm -Rf build
mkdir build

cd build && cmake \
	-DMYSQL_INC_DIR="/usr/local/mysql/include/mysql" \
	-DMYSQL_LIB_DIR="/usr/local/mysql/lib" \
	-DREDIS_INC_DIR="/usr/local/redis/inc" \
	-DREDIS_LIB_DIR="/usr/local/redis/lib" \
	.. && make
