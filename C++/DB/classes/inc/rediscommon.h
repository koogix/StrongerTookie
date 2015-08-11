#ifndef __KOOGIX_DB_REDISCOMMON_H__
#define __KOOGIX_DB_REDISCOMMON_H__

#if _USE_REDIS
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

class Redis : public std::shared_ptr<Redis>
{
public:
	class Result : public std::shared_ptr<Redis>
	{
	public:
		Result(redisContext*& context, const char* format, va_list args);
		Result();
		virtual ~Result();
		bool isEmpty();
		bool isError();
		std::string strError();
	private:
		redisReply * _reply;
		redisContext * _con;
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

#endif
#endif /* __KOOGIX_DB_REDISCOMMON_H__ */
