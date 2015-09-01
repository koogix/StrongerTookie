#ifndef __KOOGIX_DB_SQLITE3COMMON_H__
#define __KOOGIX_DB_SQLITE3COMMON_H__

#include <memory>
#include <string>
#include <unordered_map>
#include <cstdarg>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <sqlite3.h>

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


/**
 * SQLite3 数据库操作
 *
 *
 *
 */
class SQLiteFactory
{
public:
	class Connection : public std::enable_shared_from_this<Connection>
	{
	public:
		class Result : public std::enable_shared_from_this<Result>
		{
		public:
			class Row
			{
			public:
				Row(std::shared_ptr<Result> res, std::unordered_map<std::string, std::string> data);
				Row(std::shared_ptr<Result> res);
				virtual ~Row();
				operator bool() const;
				Row& operator++ ();
				std::string operator[] (unsigned int index);
				std::string operator[] (const char* name);
				std::string field(unsigned int index);
				std::string field(std::string name);
				bool isEmpty();
			private:
				std::unordered_map<std::string, std::string> _map;
				std::shared_ptr<Result> _res;
			};
			friend Row;
		public:
			Result(std::shared_ptr<Connection> conn, std::string sql);
			Result(std::shared_ptr<Connection> conn);
			virtual ~Result();
			bool isError();
			std::string strError();
			Row fetch();
		private:
			int  _error_code;
			sqlite3_stmt * _stmt;
			std::shared_ptr<Connection> _conn;
			std::unordered_map<unsigned int, std::string> _field_map;
		};
		friend Result;
	public:
		Connection(const char* filename);
		virtual ~Connection();
		
		/**
		 * 是否有错误
		 *
		 */
		bool isError();
		
		/**
		 * 显示错误信息
		 *
		 * @return std::string   错误提示信息
		 */
		std::string strError();
		
		/**
		 * 执行操作
		 *
		 */
		bool exec(const char* format, ...);
	
		/**
		 * 查询方法
		 *
		 */
		std::shared_ptr<Result> query(const char* format, ...);
		
	private:
		int    _error_code;
		char    * _message;
		sqlite3 * _handler;
	};
public:
	static std::shared_ptr<Connection> getConnection(const char* filename);
	static std::shared_ptr<Connection> getConnection();
private:
	SQLiteFactory();
	static std::unordered_map<std::string, std::shared_ptr<Connection> > arrConnection;
};

/* ~ db-ENDED ~ */
} //~: namespace db
/* ^^^^^^^^^^^^^ */

#ifndef DB_NO_NAMESPACE_KOOGIX
} //~: namespace koogix

typedef koogix::db::SQLiteFactory                           DBLITE_FAC_T;
typedef koogix::db::SQLiteFactory::Connection               DBLITE_CON_T;
typedef koogix::db::SQLiteFactory::Connection::Result       DBLITE_RES_T;
typedef koogix::db::SQLiteFactory::Connection::Result::Row  DBLITE_ROW_T;

#else //~: no namespace koogix

typedef db::SQLiteFactory                           DBLITE_FAC_T;
typedef db::SQLiteFactory::Connection               DBLITE_CON_T;
typedef db::SQLiteFactory::Connection::Result       DBLITE_RES_T;
typedef db::SQLiteFactory::Connection::Result::Row  DBLITE_ROW_T;

#endif /* DB_NO_NAMESPACE_KOOGIX */

/**
 * shared ponters
 *
 */
typedef std::shared_ptr<DBLITE_FAC_T>          DBLITE_FAC_T_PTR;
typedef std::shared_ptr<DBLITE_CON_T>          DBLITE_CON_T_PTR;
typedef std::shared_ptr<DBLITE_RES_T>          DBLITE_RES_T_PTR;
typedef std::shared_ptr<DBLITE_ROW_T>          DBLITE_ROW_T_PTR;

#endif /* __KOOGIX_DB_SQLITE3COMMON_H__ */
