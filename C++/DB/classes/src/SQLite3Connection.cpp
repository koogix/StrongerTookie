#include "sqlite3common.h"

#ifndef DB_NO_NAMESPACE_KOOGIX
using namespace koogix::db;
#else
using namespace db;
#endif /* DB_NO_NAMESPACE_KOOGIX */

SQLiteFactory::Connection::Connection(const char* filename)
	: _message    (NULL)
	, _error_code (sqlite3_open_v2(filename, &_handler, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL))
{
}
SQLiteFactory::Connection::~Connection()
{
	if (_message) sqlite3_free(_message);
	if (_handler) sqlite3_close_v2(_handler);
}

/**
 * 是否有错误
 *
 */
bool SQLiteFactory::Connection::isError()
{
	return (_error_code != SQLITE_OK);
}

/**
 * 显示错误信息
 *
 * @return std::string   错误提示信息
 */
std::string SQLiteFactory::Connection::strError()
{
	if (_message != NULL)
	{
		return _message;
	}
	return sqlite3_errmsg(_handler);
}

/**
 * 执行操作
 *
 */
bool SQLiteFactory::Connection::exec(std::string sql)
{
	return (sqlite3_exec(_handler, sql.c_str(), NULL, NULL, &_message) == SQLITE_OK);
}

/**
 * 查询方法
 *
 */
std::shared_ptr<SQLiteFactory::Connection::Result> SQLiteFactory::Connection::query(std::string sql)
{
	if (_error_code != SQLITE_OK)
	{
		return std::make_shared<SQLiteFactory::Connection::Result>(shared_from_this());
	}
	return std::make_shared<SQLiteFactory::Connection::Result>(shared_from_this(), sql);
}


