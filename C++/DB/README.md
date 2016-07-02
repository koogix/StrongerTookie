���ݿ�������߼�
===

##### MySQL���ݿ����

1. koogix::db::MyLibrary::bootstrap(argc, argv)
2. DBMY_CON_T_PTR conn = std::make_shared<DBMY_CON_T>(host, port, username, password, database);
3. bool result = conn->exec( ... [ INSERT | UPDATE | DELETE ] ...)
4. DBMY_RES_T_PTR result = conn->query( ... [ SELECT ] ... )
5. while (DBMY_ROW_T row = result->fetch()) {  }

##### SQLite3���ݿ����

1. DBLITE_CON_T_PTR sqlite = koogix::db::SQLiteFactory::getConnection()
2. bool result = sqlite->exec( ... [ INSERT | UPDATE | DELETE ] ...)
3. DBMY_RES_T_PTR result = sqlite->query( ... [ SELECT ] ... )
4. while (DBLITE_ROW_T row = result->fetch()) {  }

##### Redis���ݿ����

1. DBRD_CON_T_PTR redcon = std::make_shared<DBRD_CON_T>(host, port)
2. DBRD_RES_T_PTR buffer = redcon->command( ... [ REDIS COMMAND ] ...)
3. buffer->getInteger()
4. buffer->getString()
5. buffer->getArraySize() �� buffer->getArrayItem()
