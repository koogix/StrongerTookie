set (CMAKE_CXX_FLAGS -std=c++11)
include_directories ( inc )
# aux_source_directory (src SOURCE_LIB)
# add_library (dbutil STATIC ${SOURCE_LIB})

set (SOURCE_LIB "")

if (USE_MYSQL)
	set (SOURCE_LIB ${SOURCE_LIB} src/MyLibrary.cpp src/MyConnection.cpp src/MyResult.cpp src/MyRow.cpp)
endif (USE_MYSQL)

if (USE_REDIS)
	set (SOURCE_LIB ${SOURCE_LIB} src/RedisCon.cpp)
endif (USE_REDIS)

message (STATUS ${SOURCE_LIB})

add_library (dbutil STATIC ${SOURCE_LIB})
