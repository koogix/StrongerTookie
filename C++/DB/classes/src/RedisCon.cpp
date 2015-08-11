#include <iostream>
#include <cstdarg>
#include "rediscommon.h"

#ifndef DB_NO_NAMESPACE_KOOGIX
using namespace koogix::db;
#else
using namespace db;
#endif /* DB_NO_NAMESPACE_KOOGIX */

Redis::Redis(std::string host, unsigned int port, unsigned int database, std::string passwd)
	: _context (redisConnect(host.c_str(), port))
{
	if (_context != NULL && !_context->err)
	{
		if (! passwd.empty())
		{
			command("AUTH %s", passwd.c_str());
		}
		if (database > 0)
		{
			command("SELECT %d", database);
		}
	}
}

Redis::~Redis()
{
	if (_context != NULL)
	{
		redisFree(_context);
	}
}

std::shared_ptr<Redis::Result> Redis::command(std::string format, ...)
{
	if (_context == NULL)
	{
		return std::make_shared<Redis::Result>();
	}
	va_list args;
	va_start(args, format);
	va_end(args);
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
		return std::string();
	}
	return _context->errstr;
}

Redis::Result::Result(redisContext*& context, const char* format, va_list args)
	: _con (context)
	, _reply ((redisReply *) redisvCommand(context, format, args))
{
}
Redis::Result::Result()
	: _con (NULL)
	, _reply (NULL)
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
	if (_reply == NULL)
	{
		return true;
	}
	return (_reply->type == REDIS_REPLY_NIL);
}

bool Redis::Result::isError()
{
	if (_reply == NULL)
	{
		return (_con == NULL);
	}
	return (_reply->type == REDIS_REPLY_ERROR);
}

std::string Redis::Result::strError()
{
	if (! isError())
	{
		return std::string();
	}
	if (_reply == NULL)
	{
		return _con->errstr;
	}
	return _reply->str;
}

