#!/bin/sh

rm -Rf build
mkdir build

cd build && cmake \
	-G "MSYS Makefiles" \
	-DUSE_LLVM_CLANG=1 \
	-DUSE_REDIS=0 \
	-DMYSQL_INC_DIR="/mingw64/include/mysql" \
	-DMYSQL_LIB_DIR="/mingw64/lib" \
	.. && make
