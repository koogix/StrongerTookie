#include <iostream>
#include "mysqlcommon.h"
#include "rediscommon.h"

int main(int argc, char** argv)
{
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
	 */
//	for (DBMY_ROW_T row = result->fetch(); ! row.isEmpty(); row = result->fetch())
	for (DBMY_ROW_T row = result->fetch(); ! row.isEmpty(); ++row)
//	while (DBMY_ROW_T row = result->fetch())
	{
		std::cout << row["id"] << " " << row[1] << std::endl;
	}
	
	/**
	 * insert / update / delete
	 *
	 */
	result = conn->query("INSERT INTO `table`(`name`, `passwd`) VALUE ('test', 'test')");
	if (result->isError()) std::cout << result->strError() << std::endl;
	
	result = conn->query("UPDATE `table` SET `passwd`='password' WHERE `name`='test'");
	if (result->isError()) std::cout << result->strError() << std::endl;
	
	result = conn->query("DELETE FROM `table` WHERE `name`='test'");
	if (result->isError()) std::cout << result->strError() << std::endl;
	
	
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
	
	return 0;
}
