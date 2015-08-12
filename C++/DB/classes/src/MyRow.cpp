#include "mysqlcommon.h"

#ifndef DB_NO_NAMESPACE_KOOGIX
using namespace koogix::db;
#else
using namespace db;
#endif /* DB_NO_NAMESPACE_KOOGIX */

Connection::Result::Row::Row(std::shared_ptr<Result> res, MYSQL_ROW row)
	: _res (std::ref(res))
	, _row (row)
	, _empty_flag (row == NULL)
{
}

Connection::Result::Row::Row()
	: _res (nullptr)
	, _row (NULL)
	, _empty_flag (true)
{
}

Connection::Result::Row::~Row()
{
}

Connection::Result::Row::operator bool() const
{
	return !_empty_flag;
}

Connection::Result::Row& Connection::Result::Row::operator++ ()
{
	_row = mysql_fetch_row(_res->_result);
	
	if (_row == NULL)
	{
		_empty_flag = true;
	}
	return (*this);
}

std::string Connection::Result::Row::operator[] (unsigned int index)
{
	return field(index);
}

std::string Connection::Result::Row::operator[] (const char* name)
{
	return field(name);
}

bool Connection::Result::Row::isEmpty()
{
	return _empty_flag;
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


