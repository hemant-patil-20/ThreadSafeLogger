#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
class SafeQueue
{
private:
	condition_variable work_available;
	mutex work_mutex;
	queue<string> squeue;

public:
	void push(string str);
	string pop();
	bool empty();
};
