#include <iostream>
#include <cstdarg>
#include "rediscommon.h"

#ifndef DB_NO_NAMESPACE_KOOGIX
using namespace koogix::db;
#else
using namespace db;
#endif /* DB_NO_NAMESPACE_KOOGIX */

Redis::Redis(std::string host, unsigned int port)
	: _context (redisConnect(host.c_str(), port))
{
}

Redis::~Redis()
{
	if (_context != NULL)
	{
		redisFree(_context);
	}
}

std::shared_ptr<Redis::Result> Redis::query(std::string format, ...)
{
	if (_context == NULL)
	{
		return std::make_shared<Redis::Result>();
	}
	va_list args;
	va_start(args, format);
	va_end (args);
	return std::make_shared<Redis::Result>(_context, format.c_str(), args);
}

bool Redis::isError()
{
	if (_context != NULL && _context->err)
	{
		return true;
	}
	return false;
}

std::string Redis::strError()
{
	if (!isError())
	{
		return NULL;
	}
	return _context->errstr;
}

Redis::Result::Result(redisContext*& context, const char* format, va_list args)
	: _reply (redisCommand(context, format, args))
{
}
Redis::Result::Result()
	: _reply (NULL)
{
}
Redis::Result::~Result()
{
	if (_reply != NULL)
	{
		freeReplyObject(_reply);
	}
}

bool Redis::Result::isEmpty()
{
	return (_reply == NULL);
}
