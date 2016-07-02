#include <iostream>

#include "config.h"

#ifdef USE_SQLITE3
#	include "sqlite3common.h"
#endif /* USE_SQLITE3 */

#ifdef USE_MYSQL
#	include "mysqlcommon.h"
#endif /* USE_MYSQL */

#ifdef USE_REDIS
#	include "rediscommon.h"
#endif /* USE_REDIS */

int main(int argc, char** argv)
{
#ifdef USE_SQLITE3
	
	DBLITE_CON_T_PTR sqlite = koogix::db::SQLiteFactory::getConnection();
	if (sqlite->isError())
	{
		std::cout << sqlite->strError() << std::endl;
	}
	if (sqlite->exec("create table if not exists [user] ([id] integer primary key, [name] text, [password] text)") == false)
	{
		std::cout << sqlite->strError() << std::endl;
	}
	if (sqlite->exec("insert into [user] ([name], [password]) values (\"aa\", \"123456\")") == false)
	{
		std::cout << sqlite->strError() << std::endl;
	}
	if (sqlite->exec("insert into [user] ([name], [password]) values (\"bb\", \"123456\")") == false)
	{
		std::cout << sqlite->strError() << std::endl;
	}
	DBLITE_RES_T_PTR liteResult = sqlite->query("select * from [user]");
	while (DBLITE_ROW_T liteRow = liteResult->fetch())
	{
		std::cout << liteRow["name"] << " : " << liteRow["password"] << std::endl;
	}
	
#endif /* USE_SQLITE3 */

#ifdef USE_MYSQL
	
	/**
	 * 初始化
	 *
	 */
	koogix::db::MyLibrary::bootstrap(argc, argv);
	
	/**
	 * 连接
	 *
	 */
	DBMY_CON_T_PTR conn = std::make_shared<DBMY_CON_T>("127.0.0.1", 3306, "user", "passwd", "database");
	if (conn->isError()) std::cout << conn->strError() << std::endl;
	
	/**
	 * 查询
	 *
	 */
	DBMY_RES_T_PTR result = conn->query("SELECT * FROM `table`");
	if (result->isError()) std::cout << result->strError() << std::endl;
	
	/**
	 * 取结果
	 *
	 * 可以使用以下三种 遍历方式
	 */
//	for (DBMY_ROW_T row = result->fetch(); ! row.isEmpty(); row = result->fetch())
//	for (DBMY_ROW_T row = result->fetch(); ! row.isEmpty(); ++row)
	while (DBMY_ROW_T row = result->fetch())
	{
		std::cout << row["id"] << " " << row[1] << std::endl;
	}
	
	/**
	 * insert / update / delete
	 *
	 */
	if (! conn->exec("INSERT INTO `table`(`name`, `passwd`) VALUE ('test', 'test')"))
	{
		std::cout << result->strError() << std::endl;
	}
	else
	{
		std::cout << "new record id: " << conn->insertid() << std::endl;
	}
	if (! conn->exec("UPDATE `table` SET `passwd`='password' WHERE `name`='test'"))
	{
		std::cout << result->strError() << std::endl;
	}
	if (! conn->exec("DELETE FROM `table` WHERE `name`='test'"))
	{
		std::cout << result->strError() << std::endl;
	}
	
#endif /* USE_MYSQL */
	
#ifdef USE_REDIS

	/**
	 * REDIS连接
	 *
	 */
	DBRD_CON_T_PTR redcon = std::make_shared<DBRD_CON_T>("127.0.0.1", 6379);
	if (redcon->isError())
	{
		std::cout << "REDIS-CON-ERROR: " << redcon->strError() << std::endl;
	}
	/**
	 * 执行redis命令
	 *
	 */
	DBRD_RES_T_PTR buffer = redcon->command("ZADD ranking %d %s", "123", "aa");
	if (buffer->isError())
	{
		std::cout << "REDIS-RES-ERROR: " << buffer->strError() << std::endl;
	}
	buffer = redcon->command("DEL uuid");
	buffer = redcon->command("SETNX uuid 12345");
	std::cout << "SETNX RESULT 001: " << buffer->getInteger() << std::endl;
	buffer = redcon->command("SETNX uuid 67890");
	std::cout << "SETNX RESULT 002: " << buffer->getInteger() << std::endl;
	
#endif /* USE_REDIS */

	return 0;
}
