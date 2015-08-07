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
	
	for (DBMY_ROW_T row = result->fetch(); !row.isEmpty(); )
	{
		std::cout << row["id"] << " " << row[1] << std::endl;
	}
	DBRD_CON_T_PTR redcon = std::make_shared<DBRD_CON_T>("127.0.0.1", 6379);
	DBRD_RES_T_PTR buffer = redcon->query("SET a 1");

	return 0;
}
