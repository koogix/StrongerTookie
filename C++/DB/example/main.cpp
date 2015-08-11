#include <iostream>
#include "mysqlcommon.h"
#include "rediscommon.h"

int main(int argc, char** argv)
{
	koogix::db::MyLibrary::bootstrap(argc, argv);
	
	DBMY_CON_T_PTR conn = std::make_shared<DBMY_CON_T>("127.0.0.1", 3306, "user", "passwd", "database");
	if (conn->isError()) std::cout << conn->strError() << std::endl;
	
	DBMY_RES_T_PTR result = conn->query("SELECT * FROM `table`");
	if (result->isError()) std::cout << result->strError() << std::endl;
	
	while (DBMY_ROW_T row = result->fetch())
	{
		std::cout << row["id"] << " " << row[1] << std::endl;
	}
	#if _USE_REDIS
	DBRD_CON_T_PTR redcon = std::make_shared<DBRD_CON_T>("127.0.0.1", 6379);
	if (redcon->isError())
	{
		std::cout << "REDIS-CON-ERROR: " << redcon->strError() << std::endl;
	}
	DBRD_RES_T_PTR buffer = redcon->command("ZADD ranking %d %s", "123", "aa");
	if (buffer->isError())
	{
		std::cout << "REDIS-RES-ERROR: " << buffer->strError() << std::endl;
	}
	#endif
	return 0;
}
