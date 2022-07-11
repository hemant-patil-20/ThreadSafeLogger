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
#include "ThreadSafeLogger.h"
using namespace std;

void ThreadSafeLogger::log(string message,LOG_LEVEL level, LOG_TYPE type)
{
	if( type == FILE_LOG )
	{
		logInToFile(message,level,type);
	}
	else if( type == DB_LOG )
	{
		logInToDB(message,level,type);
	}
	else if( type == HTTP_LOG )
	{	
		logInToHTTP(message,level,type );
	}
	else if( type == CONSOLE ) 
	{
		cout<<message<<endl;
	}	
}
string ThreadSafeLogger::getLogLevelString(LOG_LEVEL level )
{
	string ll;
	switch(level)
	{
		case 0:ll="TRACE";break;
		case 1:ll="DEBUG";break;
		case 2:ll="INFO";break;
		case 3:ll="WARN";break;
		case 4:ll="ERROR";break;
	}
	return ll;
}
string ThreadSafeLogger::getLogTypeString(LOG_TYPE type )
{
	string ll;
	switch(type)
	{
		case 1:ll= "NOLOG";break;
		case 2:ll= "CONSOLE";break;
		case 3:ll= "FILE_LOG";break;
		case 4:ll= "DB_LOG";break;
		case 5:ll= "HTTP_LOG";break;
	}
	return ll;
}

void ThreadSafeLogger::logInToFile(string msg, LOG_LEVEL level, LOG_TYPE type)
{
	
	cout<<" pushing msg to file"<<endl;
	string output;
	output.reserve(msg.length());
	output.append("[");
	output.append(getLogLevelString(level));
	output.append("]");
	output.append(":[");
	output.append(getLogTypeString(type));
	output.append("]  ");
	output.append(msg);
        msgqueue.push(output);
}
void ThreadSafeLogger::logInToDB(string msg, LOG_LEVEL level, LOG_TYPE type)
{
	cout<<" pushing DB msg "<<endl;
	string output;
	output.reserve(msg.length());
	output.append("[");
	output.append(getLogLevelString(level));
	output.append("]");
	output.append(":[");
	output.append(getLogTypeString(type));
	output.append("]  ");
	output.append(msg);
        msgqueue.push(output);
}
void ThreadSafeLogger::logInToHTTP(string msg,LOG_LEVEL level, LOG_TYPE type)
{
	cout<<" pushing HTTP msg "<<endl;
	string output;
        output.reserve(msg.length());
        output.append("[");
        output.append(getLogLevelString(level));
        output.append("]");
        output.append(":[");
        output.append(getLogTypeString(type));
        output.append("]  ");
        output.append(msg);
        msgqueue.push(output);


}
void ThreadSafeLogger::logInToConsole(string msg, LOG_LEVEL level, LOG_TYPE type)
{
	cout<<" [ "<<getLogLevelString(level)<<"]"<<":["<<getLogTypeString(type)<<"] "<<msg<<endl;
}
void ThreadSafeLogger::log(string msg)
{
	cout<<" pushing msg "<<endl;
	msgqueue.push(msg);
}

ThreadSafeLogger::ThreadSafeLogger()
{
	log_file.open("log.txt"); 
	threadObj = new thread(&ThreadSafeLogger::writer, this);
	//mythread = async(launch::async,ThreadSafeLogger::writer);
	//future<void> result =  async(launch::async,writer,0);
	cout<<" calling thread safe logger constructor "<<endl;
}

ThreadSafeLogger::~ThreadSafeLogger()
{
	running = false; //set running false to let writer thread stop as soon as it completes its current cycle
	threadObj->join(); //wait until writer thread regularly exited (leaving time to flush)
	delete threadObj; //delete it
	log_file.close(); //close the log file
}
