cmake_minimum_required (VERSION 2.8)

project (DBDemo)

set (MYSQL_INC_DIR  /usr/local/mysql/include/mysql)
set (MYSQL_LIB_DIR  /usr/local/mysql/lib)
set (MYSQL_DL_LINK  mysqlclient pthread m dl)

set (REDIS_INC_DIR  /usr/local/redis/inc)
set (REDIS_LIB_DIR  /usr/local/redis/lib)
set (REDIS_DL_LINK  hiredis)

add_subdirectory (classes lib)
add_subdirectory (example bin)

add_dependencies(demo dbutil)
