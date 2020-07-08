#ifndef CONFIG_H
#define CONFIG_H

namespace sqlinfo {

        static const char *db_pool_name = "mypool";
        static const char* db_host = "127.0.0.1";
        static const int   db_port = 3306;
        static const char* db_dbname = "test_pool";
        static const char* db_username = "root";
        static const char* db_password = "12345678";
        static const int   db_maxconncnt = 4;

};



#endif
