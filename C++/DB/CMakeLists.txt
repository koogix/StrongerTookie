cmake_minimum_required (VERSION 2.8)
set (CMAKE_CXX_COMPILER clang++)

configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  )

project (DBDemo)

set (MYSQL_INC_DIR  /mingw64/include/mariadb)
set (MYSQL_LIB_DIR  /usr/local/mysql/lib)
set (MYSQL_DL_LINK  mysqlclient pthread m )

set (REDIS_INC_DIR  /usr/local/redis/inc)
set (REDIS_LIB_DIR  /usr/local/redis/lib)
#set (REDIS_DL_LINK  hiredis)

add_subdirectory (classes lib)
add_subdirectory (example bin)

add_dependencies(demo dbutil)
