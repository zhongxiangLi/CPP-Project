cmake_minimum_required(VERSION 2.8)
PROJECT(test_DBPool)

SET(SRC_LIST
    DBConn.cpp
    DBPool.cpp
    PrepareStatement.cpp
    ResultSet.cpp
	ThreadNotify.cpp
)

SET(EXECUTABLE_OUTPUT_PATH  ./)


ADD_DEFINITIONS( -g -W -Wall -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DAC_HAS_INFO
-DAC_HAS_WARNING -DAC_HAS_ERROR -DAC_HAS_CRITICAL -DTIXML_USE_STL
-DAC_HAS_DEBUG -DLINUX_DAEMON -std=c++11 -DENCRYPT)

INCLUDE_DIRECTORIES(/www/server/mysql/include/)
LINK_DIRECTORIES(/usr/lib/x86_64-linux-gnu/ /usr/lib64/mysql /www/server/mysql/lib)

ADD_EXECUTABLE(main_DBPool main_DBPool.cpp ${SRC_LIST})
ADD_EXECUTABLE(main_ThreadPool main_ThreadPool.cpp ${SRC_LIST})

TARGET_LINK_LIBRARIES(main_DBPool mysqlclient pthread) 
TARGET_LINK_LIBRARIES(main_ThreadPool mysqlclient pthread) 
