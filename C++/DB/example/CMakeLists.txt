set (DBUTIL_INC_DIR ${PROJECT_SOURCE_DIR}/classes/inc)
set (DBUTIL_LIB_DIR ${PROJECT_BINARY_DIR}/lib)

include_directories (
	${DBUTIL_INC_DIR}
	${MYSQL_INC_DIR}
	${REDIS_INC_DIR}
)
link_directories (
	${DBUTIL_LIB_DIR}
	${MYSQL_LIB_DIR}
	${REDIS_LIB_DIR}
 )
link_libraries (
	dbutil
	${SQLITE3_DL_LINK}
	${MYSQL_DL_LINK}
	${REDIS_DL_LINK}
	${LIBDL}
)

set (CMAKE_CXX_FLAGS -std=c++11)
add_executable (demo main.cpp)
