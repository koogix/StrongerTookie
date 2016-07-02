#include "sqlite3common.h"

#ifndef DB_NO_NAMESPACE_KOOGIX
using namespace koogix::db;
#else
using namespace db;
#endif /* DB_NO_NAMESPACE_KOOGIX */

#include <thread>
#include <mutex>

static std::mutex __SQLITE_FACTORY_MUTEX__;

std::unordered_map<std::string, std::shared_ptr<SQLiteFactory::Connection> > SQLiteFactory::arrConnection;

SQLiteFactory::SQLiteFactory()
{
}
std::shared_ptr<SQLiteFactory::Connection> SQLiteFactory::getConnection()
{
	return getConnection(":memory:");
}
std::shared_ptr<SQLiteFactory::Connection> SQLiteFactory::getConnection(const char* filename)
{
	if (arrConnection.find(filename) == arrConnection.end())
	{
		std::lock_guard<std::mutex> _lockit(__SQLITE_FACTORY_MUTEX__);
		if (arrConnection.find(filename) == arrConnection.end())
		{
			arrConnection[filename] = std::make_shared<SQLiteFactory::Connection>(filename);
		}
	}
	return arrConnection[filename];
}
