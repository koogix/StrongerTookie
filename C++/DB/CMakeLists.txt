cmake_minimum_required (VERSION 2.8)

# compiler setting
option (USE_LLVM_CLANG "Use llvm clang" OFF)

if (USE_LLVM_CLANG)
	set (CMAKE_C_COMPILER   clang)
	set (CMAKE_CXX_COMPILER clang++)
endif (USE_LLVM_CLANG)

project (DBDemo)

set (MYSQL_INC_DIR  /usr/local/mysql/include/mysql)
set (MYSQL_LIB_DIR  /usr/local/mysql/lib)
set (MYSQL_DL_LINK  mysqlclient pthread m dl)

set (REDIS_INC_DIR  /usr/local/redis/inc)
set (REDIS_LIB_DIR  /usr/local/redis/lib)
set (REDIS_DL_LINK  hiredis)

include (${CMAKE_ROOT}/Modules/CheckIncludeFiles.cmake)

option (USE_MYSQL "Use MySQL DB" ON)
option (USE_REDIS "Use Redis DB" ON)

if (USE_MYSQL)
	check_include_files ("mysql.h" HAVE_MYSQL_H)
endif (USE_MYSQL)

if (USE_REDIS)
	check_include_files ("hiredis.h" HAVE_HIREDIS_H)
endif (USE_REDIS)

if (NOT HAVE_MYSQL_H)
	message (WARNING "NOT found mysql.h")
endif (NOT HAVE_MYSQL_H)

if (NOT HAVE_HIREDIS_H)
	message (WARNING "NOT found hiredis.h")
endif (NOT HAVE_HIREDIS_H)

add_subdirectory (classes lib)
add_subdirectory (example bin)

add_dependencies(demo dbutil)
