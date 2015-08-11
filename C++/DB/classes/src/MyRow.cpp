#include "mysqlcommon.h"

#ifndef DB_NO_NAMESPACE_KOOGIX
using namespace koogix::db;
#else
using namespace db;
#endif /* DB_NO_NAMESPACE_KOOGIX */

Connection::Result::Row::Row(std::shared_ptr<Result> res, MYSQL_ROW row)
	: _res (res)
	, _row (row)
	, _empty_flag (row == NULL)
{
	
}

Connection::Result::Row::~Row()
{
	
}

bool Connection::Result::Row::isEmpty()
{
	return _empty_flag;
}

std::string Connection::Result::Row::operator [] (unsigned int index)
{
	return field(index);
}

std::string Connection::Result::Row::operator [] (const char* name)
{
	return field(name);
}

Connection::Result::Row::operator bool() const
{
	return !_empty_flag;
}

std::string Connection::Result::Row::field(unsigned int index)
{
	if (_empty_flag)
	{
		return std::string();
	}
	if (index >= _res->_field_num)
	{
		return std::string();
	}
	return _row[index];
}

std::string Connection::Result::Row::field(std::string name)
{
	std::unordered_map<std::string, unsigned int>::const_iterator it;
	it = _res->_field_map.find(name);
	if (it == _res->_field_map.end())
	{
		return std::string();
	}
	return field(it->second);
}


