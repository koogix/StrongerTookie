#include "sqlite3common.h"

#ifndef DB_NO_NAMESPACE_KOOGIX
using namespace koogix::db;
#else
using namespace db;
#endif /* DB_NO_NAMESPACE_KOOGIX */

SQLiteFactory::Connection::Result::Result(std::shared_ptr<Connection> conn)
	: _conn (std::ref(conn))
	, _stmt (NULL)
	, _error_code(SQLITE_OK)
	, _field_map  (std::unordered_map<unsigned int, std::string>())
{
}

SQLiteFactory::Connection::Result::Result(std::shared_ptr<Connection> conn, std::string sql)
	: _conn (std::ref(conn))
	, _error_code (sqlite3_prepare_v2(conn->_handler, sql.c_str(), sql.size(), &_stmt, NULL))
	, _field_map  (std::unordered_map<unsigned int, std::string>())
{
}

SQLiteFactory::Connection::Result::~Result()
{
	if (_stmt) sqlite3_finalize(_stmt);
}

bool SQLiteFactory::Connection::Result::isError()
{
	return (_error_code != SQLITE_OK);
}

std::string SQLiteFactory::Connection::Result::strError()
{
	return sqlite3_errmsg(_conn->_handler);
}

SQLiteFactory::Connection::Result::Row SQLiteFactory::Connection::Result::fetch()
{
	if (sqlite3_step(_stmt) != SQLITE_ROW)
	{
		return SQLiteFactory::Connection::Result::Row(shared_from_this());
	}
	if (_field_map.empty())
	{
		int rc = sqlite3_column_count(_stmt);
		for (int i = 0; i < rc; i++)
		{
			_field_map[i] = sqlite3_column_name(_stmt, i);
		}
	}
	std::unordered_map<std::string, std::string> data;
	for (auto it : _field_map)
	{
		data[it.second] = (const char *) sqlite3_column_text(_stmt, it.first);
	}
	return SQLiteFactory::Connection::Result::Row(shared_from_this(), data);
}

