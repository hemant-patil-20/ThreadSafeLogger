#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
//enum class log_level : uint8_t { TRACE=0,DEBUG=1,INFO=2,WARN=3,ERROR=4};
enum log_level { TRACE=0,DEBUG=1,INFO=2,WARN=3,ERROR=4};
//class SafeQueue;

class SafeQueue
{
        condition_variable work_available;
        mutex work_mutex;
        queue<std::string> squeue;

public:
        void push(std::string string)
        {
                std::unique_lock<std::mutex> lock(work_mutex);

                bool was_empty = squeue.empty();
                squeue.push(string);

                lock.unlock();

                if (was_empty)
                {
                        work_available.notify_one();
                }
        }

        string pop()
        {
                std::unique_lock<std::mutex> lock(work_mutex);
                while (squeue.empty())
                {
                        work_available.wait(lock);
                }

                string tmp = squeue.front();
                squeue.pop();
                return tmp;
        }
        bool empty()
        {       return squeue.empty(); }
};

class ThreadSafeLogger
{
private:
    SafeQueue msgqueue;
    ofstream log_file;
    thread* mythread = nullptr;
    bool running = true;

        void writer()
        {
                while (running || !msgqueue.empty())
                {
                        //when running is set to false writer reaches its end, letting the main thread get past "join"
                        log_file << msgqueue.pop() << std::endl << std::flush;
                        //std::this_thread::sleep_for(std::chrono::seconds(1));
                }
        }

public:
        void log(std::string message,log_level level)
        {
                cout<<" level is = "<<level<<endl;
        //      if( level < 0 || level > 4 )
        //              return;
                string output;
                output.reserve(message.length()+64);
        //      output.append(timestamp());
                output.append("[");
                output.append(level);
                output.append("]");
                output.append(" ");
                output.append(message);
                output.append("\n");
        //      output.push_back("\n");
                log(output);
                cout<<" output is "<<output<<endl;
                //Logger.queue.push(string);
        }
        void log(string msg)
        {
                cout<<" pushing msg "<<endl;
                msgqueue.push(msg);
        }

        ThreadSafeLogger()
        {
                log_file.open("log.txt");
                mythread = new thread(&ThreadSafeLogger::writer, this);
                cout<<" calling thread safe logger constructor "<<endl;
        }

        ~ThreadSafeLogger()
        {
                running = false; //set running false to let writer thread stop as soon as it completes its current cycle
                mythread->join(); //wait until writer thread regularly exited (leaving time to flush)
                delete mythread; //delete it
                log_file.close(); //close the log file
        }
};
int main()
{
        ThreadSafeLogger SL;

        SL.log(" THis is warning log ",log_level::WARN);

        SL.log(" this is error log ",log_level::ERROR);

        SL.log(" this is debug log ",log_level::DEBUG);

        return 0;
}
