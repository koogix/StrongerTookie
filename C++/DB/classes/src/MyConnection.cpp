#include "mysqlcommon.h"

#ifndef DB_NO_NAMESPACE_KOOGIX
using namespace koogix::db;
#else
using namespace db;
#endif /* DB_NO_NAMESPACE_KOOGIX */

Connection::Connection()
	: _connection (mysql_init(NULL))
	, _error_flag (false)
{
}

Connection::~Connection()
{
	if (_connection != NULL)
	{
		mysql_close(_connection);
	}
}

Connection::Connection(const char* host, unsigned int port, const char* user, const char* passwd, const char* database, unsigned long flags)
	: _connection (mysql_init(NULL))
	, _error_flag (false)
{
	connect(host, port, user, passwd, database, flags);
}
Connection::Connection(const char* host, unsigned int port, const char* user, const char* passwd, const char* database)
	: _connection (mysql_init(NULL))
	, _error_flag (false)
{
	connect(host, port, user, passwd, database);
}
Connection::Connection(const char* host, unsigned int port, const char* user, const char* passwd)
	: _connection (mysql_init(NULL))
	, _error_flag (false)
{
	connect(host, port, user, passwd);
}
Connection::Connection(const char* unix_socket, const char* user, const char* passwd, const char* database, unsigned long flags)
	: _connection (mysql_init(NULL))
	, _error_flag (false)
{
	connect(unix_socket, user, passwd, database, flags);
}
Connection::Connection(const char* unix_socket, const char* user, const char* passwd, const char* database)
	: _connection (mysql_init(NULL))
	, _error_flag (false)
{
	connect(unix_socket, user, passwd, database);
}
Connection::Connection(const char* unix_socket, const char* user, const char* passwd)
	: _connection (mysql_init(NULL))
	, _error_flag (false)
{
	connect(unix_socket, user, passwd);
}

Connection::Connection(std::string host, unsigned int port, std::string user, std::string passwd, std::string database, unsigned long flags)
	: _connection (mysql_init(NULL))
	, _error_flag (false)
{
	connect(host, port, user, passwd, database, flags);
}
Connection::Connection(std::string host, unsigned int port, std::string user, std::string passwd, std::string database)
	: _connection (mysql_init(NULL))
	, _error_flag (false)
{
	connect(host, port, user, passwd, database);
}
Connection::Connection(std::string host, unsigned int port, std::string user, std::string passwd)
	: _connection (mysql_init(NULL))
	, _error_flag (false)
{
	connect(host, port, user, passwd);
}
Connection::Connection(std::string unix_socket, std::string user, std::string passwd, std::string database, unsigned long flags)
	: _connection (mysql_init(NULL))
	, _error_flag (false)
{
	connect(unix_socket, user, passwd, database, flags);
}
Connection::Connection(std::string unix_socket, std::string user, std::string passwd, std::string database)
	: _connection (mysql_init(NULL))
	, _error_flag (false)
{
	connect(unix_socket, user, passwd, database);
}
Connection::Connection(std::string unix_socket, std::string user, std::string passwd)
	: _connection (mysql_init(NULL))
	, _error_flag (false)
{
	connect(unix_socket, user, passwd);
}

bool Connection::connect(const char* host, unsigned int port, const char* user, const char* passwd, const char* database, unsigned long flags)
{
	if (mysql_real_connect(_connection, host, user, passwd, database, port, NULL, flags) == NULL)
	{
		_error_flag = true;
		return false;
	}
	return true;
}
bool Connection::connect(const char* host, unsigned int port, const char* user, const char* passwd, const char* database)
{
	return connect(host, port, user, passwd, database, 0);
}
bool Connection::connect(const char* host, unsigned int port, const char* user, const char* passwd)
{
	return connect(host, port, user, passwd, NULL, 0);
}
bool Connection::connect(const char* unix_socket, const char* user, const char* passwd, const char* database, unsigned long flags)
{
	if (mysql_real_connect(_connection, NULL, user, passwd, database, 0, unix_socket, flags) == NULL)
	{
		_error_flag = true;
		return false;
	}
	return true;
}
bool Connection::connect(const char* unix_socket, const char* user, const char* passwd, const char* database)
{
	return connect(unix_socket, user, passwd, database, 0);
}
bool Connection::connect(const char* unix_socket, const char* user, const char* passwd)
{
	return connect(unix_socket, user, passwd, NULL, 0);
}

bool Connection::connect(std::string host, unsigned int port, std::string user, std::string passwd, std::string database, unsigned long flags)
{
	return connect(host.c_str(), port, user.c_str(), passwd.c_str(), database.c_str(), flags);
}
bool Connection::connect(std::string host, unsigned int port, std::string user, std::string passwd, std::string database)
{
	return connect(host.c_str(), port, user.c_str(), passwd.c_str(), database.c_str(), 0);
}
bool Connection::connect(std::string host, unsigned int port, std::string user, std::string passwd)
{
	return connect(host.c_str(), port, user.c_str(), passwd.c_str(), NULL, 0);
}
bool Connection::connect(std::string unix_socket, std::string user, std::string passwd, std::string database, unsigned long flags)
{
	return connect(unix_socket.c_str(), user.c_str(), passwd.c_str(), database.c_str(), flags);
}
bool Connection::connect(std::string unix_socket, std::string user, std::string passwd, std::string database)
{
	return connect(unix_socket.c_str(), user.c_str(), passwd.c_str(), database.c_str(), 0);
}
bool Connection::connect(std::string unix_socket, std::string user, std::string passwd)
{
	return connect(unix_socket.c_str(), user.c_str(), passwd.c_str(), NULL, 0);
}

/**
 * 是否有错误发生
 *
 */
bool Connection::isError()
{
	return _error_flag;
}

/**
 * 错误提示信息
 *
 */
std::string Connection::strError()
{
	if (_connection == NULL)
	{
		return std::string("NOT CONNECTED");
	}
	return mysql_error(_connection);
}
	
/**
 * 执行方法
 *
 */
bool Connection::exec(const char* format, ...)
{
	if (_connection == NULL)
	{
		return false;
	}
	std::string sql;
	va_list args;
	va_start(args, format);
	int len = vsnprintf(NULL, 0, format, args);
	sql.resize(++len);
	vsnprintf((char *)sql.data(), len, format, args);
	va_end(args);
	return (mysql_real_query(_connection, sql.c_str(), sql.size()) == 0);
}

/**
 * 查询方法
 *
 */
std::shared_ptr<Connection::Result> Connection::query(const char* format, ...)
{
	std::string sql;
	va_list args;
	va_start(args, format);
	int len = vsnprintf(NULL, 0, format, args);
	sql.resize(++len);
	vsnprintf((char *)sql.data(), len, format, args);
	va_end(args);
	if (_connection == NULL || mysql_real_query(_connection, sql.c_str(), sql.size()))
	{
		return std::make_shared<Connection::Result>(shared_from_this(), (MYSQL_RES *) NULL);
	}
	return std::make_shared<Connection::Result>(shared_from_this(), mysql_use_result(_connection));
}

/**
 * 最后新增id
 *
 */
int Connection::insertid()
{
	if (_connection == NULL)
	{
		return 0;
	}
	return mysql_insert_id(_connection);
}

