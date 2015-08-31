set (CMAKE_CXX_FLAGS -std=c++11)
include_directories ( inc )

set (SOURCE_LIB "")

if (USE_SQLITE3)
	set (SOURCE_LIB ${SOURCE_LIB} src/SQLite3Factory.cpp src/SQLite3Connection.cpp src/SQLite3Result.cpp src/SQLite3Row.cpp)
endif (USE_SQLITE3)

if (USE_MYSQL)
	set (SOURCE_LIB ${SOURCE_LIB} src/MyLibrary.cpp src/MyConnection.cpp src/MyResult.cpp src/MyRow.cpp)
endif (USE_MYSQL)

if (USE_REDIS)
	set (SOURCE_LIB ${SOURCE_LIB} src/RedisCon.cpp)
endif (USE_REDIS)

add_library (dbutil STATIC ${SOURCE_LIB})
