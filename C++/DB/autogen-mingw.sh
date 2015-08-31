#!/bin/sh

rm -Rf build
mkdir build

cd build && cmake \
	-G "MSYS Makefiles" \
	-D USE_LLVM_CLANG=0 \
	-D USE_REDIS=0 \
	-D USE_SQLITE3=1 \
	-D SQLITE3_INC_DIR="/mingw64/include" \
	-D SQLITE3_LIB_DIR="/mingw64/lib" \
	-D MYSQL_INC_DIR="/mingw64/include/mysql" \
	-D MYSQL_LIB_DIR="/mingw64/lib" \
	.. && make
