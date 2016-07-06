#include "sqlite3common.h"

#ifndef DB_NO_NAMESPACE_KOOGIX
using namespace koogix::db;
#else
using namespace db;
#endif /* DB_NO_NAMESPACE_KOOGIX */

SQLiteFactory::SQLiteFactory::Connection::Result::Row::Row(std::shared_ptr<Result> res, std::unordered_map<std::string, std::string> data)
	: _res (std::ref(res))
	, _map (data)
{
}

SQLiteFactory::SQLiteFactory::Connection::Result::Row::Row(std::shared_ptr<Result> res)
	: _res (std::ref(res))
	, _map (std::unordered_map<std::string, std::string>())
{
}

SQLiteFactory::SQLiteFactory::Connection::Result::Row::~Row()
{	
}

SQLiteFactory::Connection::Result::Row::operator bool() const
{
	return ! _map.empty();
}

SQLiteFactory::Connection::Result::Row& SQLiteFactory::Connection::Result::Row::operator++ ()
{
	Row row = _res->fetch();
	return (*this);
}

std::string SQLiteFactory::Connection::Result::Row::operator[] (unsigned int index)
{
	return field(index);
}

std::string SQLiteFactory::Connection::Result::Row::operator[] (const char* name)
{
	return field(name);
}

bool SQLiteFactory::Connection::Result::Row::isEmpty()
{
	return _map.empty();
}

std::string SQLiteFactory::Connection::Result::Row::field(unsigned int index)
{
	if (_res->_field_map.find(index) == _res->_field_map.end())
	{
		return std::string();
	}
	return field(_res->_field_map[index]);
}

std::string SQLiteFactory::Connection::Result::Row::field(std::string name)
{
	if (_map.find(name) == _map.end())
	{
		return std::string();
	}
	return _map[name];
}
