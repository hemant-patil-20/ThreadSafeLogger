#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <future>
#include <atomic>
#include "safeQueue.h"
using namespace std;
//using namespace QUEUE;

enum LOG_LEVEL
{
 TRACE=0,
 DEBUG=1,
 INFO=2,
 WARN=3,
 ERROR=4
};

enum LOG_TYPE
{
 NO_LOG = 1,
 CONSOLE = 2,
 FILE_LOG = 3,
 DB_LOG = 4,
 HTTP_LOG = 5
};



class ThreadSafeLogger:public SafeQueue
{
private:
	SafeQueue msgqueue;
    	ofstream log_file;

    	thread* threadObj = nullptr;
	atomic<bool> running{1};

        void writer()
        {
                while (running || !msgqueue.empty())
                {
                        //when running is set to false writer reaches its end, letting the main thread get past "join"
                        log_file << msgqueue.pop() << endl << flush;
                        //this_thread::sleep_for(chrono::seconds(1));
                }
        }
public:
	ThreadSafeLogger();
	~ThreadSafeLogger();

	void log(string,LOG_LEVEL,LOG_TYPE);
	void log( string );
	void logInToFile(string, LOG_LEVEL, LOG_TYPE );
	void logInToDB(string, LOG_LEVEL, LOG_TYPE );
	void logInToHTTP(string, LOG_LEVEL, LOG_TYPE );
	void logInToConsole(string,LOG_LEVEL, LOG_TYPE );

	string getLogLevelString(LOG_LEVEL);
	string getLogTypeString(LOG_TYPE);

};
