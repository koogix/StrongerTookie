#ifndef __KOOGIX_DB_MYSQLCOMMON_H__
#define __KOOGIX_DB_MYSQLCOMMON_H__

#include <memory>
#include <string>
#include <unordered_map>
#include <cstdarg>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <mysql.h>

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
 * MyLibrary代理
 *
 * 用于调用 mysql_library_init()
 * 及结束是自动调用 mysql_library_end()
 *
 */
class MyLibrary
{
public:
	/**
	 * 释构函数
	 *
	 * 调用释构函数时
	 * 自动调用 mysql_library_end()
	 *
	 */
	virtual ~MyLibrary();
	
	/**
	 * 启动 mysql_library_init()
	 *
	 * @param  int     argc
	 * @param  char**  argv
	 * @param  char**  groups
	 * @return void
	 */
	static void bootstrap(int argc, char** argv);
	static void bootstrap(int argc, char** argv, char** groups);
private:
	/**
	 * private
	 *
	 * 只允许调用一次
	 * 调用 mysql_library_init()
	 */
	MyLibrary(int argc, char** argv, char** groups);
	
	/**
	 * 单例对象
	 *
	 */
	static std::shared_ptr<MyLibrary> mpInstance;
};

/**
 * MySQL连接对象
 *
 * 使用 mysql_real_connect()
 * _connection 保存MySQL连接
 *
 */
class Connection : public std::enable_shared_from_this<Connection>
{
public:
	/**
	 * MySQL查询结果对象
	 *
	 * mysql_use_result()
	 * _result 保存结果
	 *
	 */
	class Result : public std::enable_shared_from_this<Result>
	{
	public:
		/**
		 * MySQL记录集对象
		 *
		 * 对应 MYSQL_ROW 读取
		 *
		 */
		class Row
		{
		public:
			Row(std::shared_ptr<Result> res, MYSQL_ROW row);
			Row();
			virtual ~Row();
			operator bool() const;
			Row& operator++ ();
			std::string operator[] (unsigned int index);
			std::string operator[] (const char* name);
			std::string field(unsigned int index);
			std::string field(std::string name);
			bool isEmpty();
		private:
			MYSQL_ROW _row;
			bool      _empty_flag;
			std::shared_ptr<Result> _res;
		};
		friend Row;
	public:
		Result(std::shared_ptr<Connection> conn, MYSQL_RES * res);
		virtual ~Result();
		bool isError();
		std::string strError();
		Row fetch();
	private:
		MYSQL_RES    * _result;
		MYSQL_FIELD  * _field_item;
		unsigned int   _field_num;
		bool           _error_flag;
		std::shared_ptr<Connection> _conn;
		std::unordered_map<std::string, unsigned int> _field_map;
	};
	friend Result;
	friend Result::Row;
public:
	Connection();
	virtual ~Connection();
	
	/**
	 * 构造函数
	 *
	 * 具体参数定义参考 connect() 方法
	 *
	 */
	Connection(const char* host, unsigned int port, const char* user, const char* passwd, const char* database, unsigned long flags);
	Connection(const char* host, unsigned int port, const char* user, const char* passwd, const char* database);
	Connection(const char* host, unsigned int port, const char* user, const char* passwd);
	Connection(const char* unix_socket, const char* user, const char* passwd, const char* database, unsigned long flags);
	Connection(const char* unix_socket, const char* user, const char* passwd, const char* database);
	Connection(const char* unix_socket, const char* user, const char* passwd);
	
	Connection(std::string host, unsigned int port, std::string user, std::string passwd, std::string database, unsigned long flags);
	Connection(std::string host, unsigned int port, std::string user, std::string passwd, std::string database);
	Connection(std::string host, unsigned int port, std::string user, std::string passwd);
	Connection(std::string unix_socket, std::string user, std::string passwd, std::string database, unsigned long flags);
	Connection(std::string unix_socket, std::string user, std::string passwd, std::string database);
	Connection(std::string unix_socket, std::string user, std::string passwd);
	
	/**
	 * 连接服务器
	 *
	 * 服务器地址与端口的连接方式
	 * 或 unix_socket 连接方式
	 * 只能取其一
	 * 账号与密码为必须的
	 * database 与 flags 可选
	 *
	 * 具体参数定义<其中const char* 与 std::string 定义一致>：
	 *
	 * --
	 * @param const char*  host          服务器地址
	 * @param unsigned int port          服务器端口
	 * -- or
	 * @param const char*  unix_socket  服务器的SOCKET文件
	 * --
	 * @param const char* user          连接服务器账号
	 * @param const char* passwd        服务器验证密码
	 * --
	 * @param const char*   database    数据库名称
	 * @param unsigned long flags       连接标志
	 *
	 * @return bool     是否成功连接
	 */
	bool connect(const char* host, unsigned int port, const char* user, const char* passwd, const char* database, unsigned long flags);
	bool connect(const char* host, unsigned int port, const char* user, const char* passwd, const char* database);
	bool connect(const char* host, unsigned int port, const char* user, const char* passwd);
	bool connect(const char* unix_socket, const char* user, const char* passwd, const char* database, unsigned long flags);
	bool connect(const char* unix_socket, const char* user, const char* passwd, const char* database);
	bool connect(const char* unix_socket, const char* user, const char* passwd);
	
	bool connect(std::string host, unsigned int port, std::string user, std::string passwd, std::string database, unsigned long flags);
	bool connect(std::string host, unsigned int port, std::string user, std::string passwd, std::string database);
	bool connect(std::string host, unsigned int port, std::string user, std::string passwd);
	bool connect(std::string unix_socket, std::string user, std::string passwd, std::string database, unsigned long flags);
	bool connect(std::string unix_socket, std::string user, std::string passwd, std::string database);
	bool connect(std::string unix_socket, std::string user, std::string passwd);
	
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
	 * 执行方法
	 *
	 */
	bool exec(const char* format, ...);
	
	/**
	 * 查询方法
	 *
	 */
	std::shared_ptr<Result> query(const char* format, ...);
	
	/**
	 * 最后新增的id
	 *
	 */
	int insertid();
	
private:
	MYSQL * _connection;
	bool    _error_flag;
};

/* ~ db-ENDED ~ */
} //~: namespace db
/* ^^^^^^^^^^^^^ */

#ifndef DB_NO_NAMESPACE_KOOGIX
} //~: namespace koogix

typedef koogix::db::Connection                DBMY_CON_T;
typedef koogix::db::Connection::Result        DBMY_RES_T;
typedef koogix::db::Connection::Result::Row   DBMY_ROW_T;

#else //~: no namespace koogix

typedef db::Connection               DBMY_CON_T;
typedef db::Connection::Result       DBMY_RES_T;
typedef db::Connection::Result::Row  DBMY_ROW_T;

#endif /* DB_NO_NAMESPACE_KOOGIX */

/**
 * shared ponters
 *
 */
typedef std::shared_ptr<DBMY_CON_T>  DBMY_CON_T_PTR;
typedef std::shared_ptr<DBMY_RES_T>  DBMY_RES_T_PTR;
typedef std::shared_ptr<DBMY_ROW_T>  DBMY_ROW_T_PTR;

#endif /* __KOOGIX_DB_MYSQLCOMMON_H__ */
