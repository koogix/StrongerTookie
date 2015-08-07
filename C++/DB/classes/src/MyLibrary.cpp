#include "mysqlcommon.h"

#ifndef DB_NO_NAMESPACE_KOOGIX
using namespace koogix::db;
#else
using namespace db;
#endif /* DB_NO_NAMESPACE_KOOGIX */

#include <thread>
#include <mutex>

static std::mutex __MY_LIBRARY_MUTEX__;

std::shared_ptr<MyLibrary> MyLibrary::mpInstance = NULL;

MyLibrary::MyLibrary(int argc, char** argv, char** groups)
{
	mysql_library_init(argc, argv, groups);
}

MyLibrary::~MyLibrary()
{
	mysql_library_end();
}

void MyLibrary::bootstrap(int argc, char** argv)
{
	bootstrap(argc, argv, NULL);
}

void MyLibrary::bootstrap(int argc, char** argv, char** groups)
{
	if (mpInstance != NULL)
	{
		return;
	}
	std::lock_guard<std::mutex> lockit(__MY_LIBRARY_MUTEX__);
	
	if (mpInstance != NULL)
	{
		return;
	}
	mpInstance = std::shared_ptr<MyLibrary>(new MyLibrary(argc, argv, groups));
}
