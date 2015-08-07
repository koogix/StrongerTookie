#include <iostream>
#include "mysqlcommon.h"

int main(int argc, char** argv)
{
	koogix::db::MyLibrary::bootstrap(argc, argv);
	
	DBMY_CON_T_PTR conn = std::make_shared<DBMY_CON_T>("192.168.186.168", 3306, "developer", "123456", "good_voice");
	if (conn->isError()) std::cout << conn->strError() << std::endl;
	
	DBMY_RES_T_PTR result = conn->query("SELECT * FROM `ViewVoiceSumPersonDay`");
	if (result->isError()) std::cout << result->strError() << std::endl;
	 
	// while (DBMY_ROW_T row = result->fetch())
	while (true)
	{
		DBMY_ROW_T row = result->fetch();
		if (row.isEmpty())
		{
			break;
		}
		std::cout << row[0] << std::endl;
	}
	
	return 0;
}
