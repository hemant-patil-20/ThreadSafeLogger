#include "ThreadSafeLogger.h"
using namespace std;

int main()
{
        ThreadSafeLogger SL;

        SL.log(" THis is warning log ",LOG_LEVEL::WARN,LOG_TYPE::FILE_LOG);

        SL.log(" this is error log ",LOG_LEVEL::ERROR,LOG_TYPE::DB_LOG);

        SL.log(" this is debug log ",LOG_LEVEL::DEBUG,LOG_TYPE::HTTP_LOG);
        
	SL.log(" this is console log ",LOG_LEVEL::DEBUG,LOG_TYPE::CONSOLE);

        return 0;
}

