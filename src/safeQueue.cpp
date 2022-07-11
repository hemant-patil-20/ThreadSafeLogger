#include"safeQueue.h"

void SafeQueue::push(string str)
{
	unique_lock<mutex> lock(work_mutex);

	bool was_empty = squeue.empty();
	squeue.push(str);

	lock.unlock();

	if (was_empty)
	{
		work_available.notify_one();
	}
}

string SafeQueue::pop()
{
	unique_lock<mutex> lock(work_mutex);
	while (squeue.empty())
	{
		work_available.wait(lock);
	}

	string tmp = squeue.front();
	squeue.pop();
	return tmp;
}
bool SafeQueue::empty()
{
	unique_lock<mutex> lock(work_mutex);
	bool checkEmpty(false);
	checkEmpty = squeue.empty();
	lock.unlock();
	return checkEmpty;
}

