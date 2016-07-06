#include "mysqlcommon.h"

#ifndef DB_NO_NAMESPACE_KOOGIX
using namespace koogix::db;
#else
using namespace db;
#endif /* DB_NO_NAMESPACE_KOOGIX */

Connection::Result::Result(std::shared_ptr<Connection> conn, MYSQL_RES * res)
	: _conn   (std::ref(conn))
	, _result (res)
	, _error_flag (res == NULL)
	, _field_num  (0)
	, _field_item (NULL)
	, _field_map  (std::unordered_map<std::string, unsigned int>())
{
	if (!_error_flag)
	{
		_field_num  = mysql_num_fields(_result);
		_field_item = mysql_fetch_fields(_result);
		for (int i = 0; i < _field_num; i++)
		{
			_field_map[_field_item[i].name] = i;
		}
	}
}

Connection::Result::~Result()
{
	if (_result != NULL)
	{
		mysql_free_result(_result);
	}
}

bool Connection::Result::isError()
{
	return _error_flag;
}

std::string Connection::Result::strError()
{
	return mysql_error(_conn->_connection);
}

Connection::Result::Row Connection::Result::fetch()
{
	MYSQL_ROW row = NULL;
	
	if (_result != NULL)
	{
		row = mysql_fetch_row(_result);
	}
	return Connection::Result::Row(shared_from_this(), row);
}
