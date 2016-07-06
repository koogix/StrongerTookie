#ifndef __KOOGIX_DB_REDISCOMMON_H__
#define __KOOGIX_DB_REDISCOMMON_H__

#include <memory>
#include <string>
#include <unordered_map>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <hiredis.h>

#ifdef __cplusplus
} //~: __cplusplus
#endif /* __cplusplus */

#ifndef DB_NO_NAMESPACE_KOOGIX
/**
 * namespace koogix
 */
namespace koogix {
#endif /* DB_NO_NAMESPACE_KOOGIX */

/**
 * namespace db
 */
namespace db {
/* ~ db-BEGIN ~ */

class Redis : public std::enable_shared_from_this<Redis>
{
public:
	class Result : public std::enable_shared_from_this<Result>
	{
	public:
		Result(std::shared_ptr<Redis> redis, const char* format, va_list args);
		Result(std::shared_ptr<Redis> redis, redisReply* reply);
		Result(std::shared_ptr<Redis> redis);
		virtual ~Result();
		bool isEmpty();
		bool isError();
		std::string strError();
		bool isNil();
		bool isInteger();
		bool isString();
		bool isArray();
		bool isStatus();
		long long getInteger();
		std::string getString();
		size_t getArraySize();
		std::shared_ptr<Result> getArrayItem(size_t index);
	private:
		redisReply * _reply;
		std::shared_ptr<Redis> _redis;
	};
	friend Result;
public:
	Redis(std::string host, unsigned int port, unsigned int database = 0, std::string passwd = "");
	virtual ~Redis();
	std::shared_ptr<Result> command(std::string format, ...);
	bool isError();
	std::string strError();
private:
	redisContext * _context;
};

/* ~ db-ENDED ~ */
} //~: namespace db
/* ^^^^^^^^^^^^^ */

#ifndef DB_NO_NAMESPACE_KOOGIX
} //~: namespace koogix

typedef koogix::db::Redis          DBRD_CON_T;
typedef koogix::db::Redis::Result  DBRD_RES_T;

#else //~: no namespace koogix

typedef db::Redis          DBRD_CON_T;
typedef db::Redis::Result  DBRD_RES_T;

#endif /* DB_NO_NAMESPACE_KOOGIX */

/**
 * shared pointers
 *
 */
typedef std::shared_ptr<DBRD_CON_T>  DBRD_CON_T_PTR;
typedef std::shared_ptr<DBRD_RES_T>  DBRD_RES_T_PTR;

#endif /* __KOOGIX_DB_REDISCOMMON_H__ */
