set (CMAKE_CXX_FLAGS -std=c++11)
include_directories (
	${MYSQL_INC_DIR}
	${REDIS_INC_DIR}
	inc
)
link_directories (
	${MYSQL_LIB_DIR}
	${REDIS_LIB_DIR}
)
aux_source_directory (src SOURCE_LIB)
add_library (dbutil STATIC ${SOURCE_LIB})