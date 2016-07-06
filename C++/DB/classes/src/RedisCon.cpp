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
	}//~ connect and select redis DB !!!
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
	if (_context == NULL || _context->err)
	{
		return std::make_shared<Redis::Result>(shared_from_this());
	}
	va_list args;
	va_start(args, format);
	va_end(args);
	return std::make_shared<Redis::Result>(shared_from_this(), format.c_str(), args);
}

bool Redis::isError()
{
	if (_context == NULL)
	{
		return true;
	}
	if (_context->err)
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
	if (_context == NULL)
	{
		return std::string("REDIS NO CONNECT");
	}
	return _context->errstr;
}

Redis::Result::Result(std::shared_ptr<Redis> redis, const char* format, va_list args)
	: _reply ((redisReply *) redisvCommand(redis->_context, format, args))
	, _redis (std::ref(redis))
{
}
Redis::Result::Result(std::shared_ptr<Redis> redis, redisReply* reply)
	: _reply (reply)
	, _redis (std::ref(redis))
{
}
Redis::Result::Result(std::shared_ptr<Redis> redis)
	: _reply (NULL)
	, _redis (std::ref(redis))
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
		return (_redis->isError());
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
		return _redis->strError();
	}
	return _reply->str;
}

bool Redis::Result::isNil()
{
	if (_reply == NULL)
	{
		return false;
	}
	return (_reply->type == REDIS_REPLY_NIL);
}

bool Redis::Result::isInteger()
{
	if (_reply == NULL)
	{
		return false;
	}
	return (_reply->type == REDIS_REPLY_INTEGER);
}

bool Redis::Result::isString()
{
	if (_reply == NULL)
	{
		return false;
	}
	return (_reply->type == REDIS_REPLY_STRING);
}

bool Redis::Result::isArray()
{
	if (_reply == NULL)
	{
		return false;
	}
	return (_reply->type == REDIS_REPLY_ARRAY);
}

bool Redis::Result::isStatus()
{
	if (_reply == NULL)
	{
		return false;
	}
	return (_reply->type == REDIS_REPLY_STATUS);
}

long long Redis::Result::getInteger()
{
	if (isError())
	{
		return 0;
	}
	return _reply->integer;
}
std::string Redis::Result::getString()
{
	if (isError())
	{
		return std::string();
	}
	return _reply->str;
}
size_t Redis::Result::getArraySize()
{
	if (isError())
	{
		return 0;
	}
	return _reply->elements;
}
std::shared_ptr<Redis::Result> Redis::Result::getArrayItem(size_t index)
{
	if (isError() || index > _reply->elements)
	{
		return std::make_shared<Redis::Result>(_redis);
	}
	return std::make_shared<Redis::Result>(_redis, _reply->element[index]);
}
