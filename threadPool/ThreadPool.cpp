#include <iostream>
#include "./ThreadPool.h"


void ThreadPool::makeThread(const int th_num)
{
	numOfWorkers = th_num;
	for (int i = 0; i < th_num; i++)
	{
		auto t = std::thread{ &ThreadPool::work, this, i };
		workers.push_back(std::move(t));
	}
}

void ThreadPool::processJob(const Job& job)
{
	std::unique_lock<std::mutex> lock(mx);
	jobQueue.push(job);
	lock.unlock();

	sendJob(job);
}

void ThreadPool::sendJob(const Job& job)
{
	cond.notify_one();
}

void ThreadPool::work(int idx)
{
	printf("%d Thread start\n", idx);
	while (true)
	{
		if (finished)
			break;

		printf("%d Thread wating\n", idx);

		std::unique_lock<std::mutex> lock(mx);
		cond.wait(lock);

		if (finished)
			break;

		printf("%d Thread wake up\n", idx);

		const auto job = jobQueue.front();
		jobQueue.pop();
		lock.unlock();

		processJob(job);
		printf("%d Thread worked\n", idx);

	}
	printf("%d Thread end\n", idx);
}

bool ThreadPool::doJob(const Job& job)
{
	// Process job
	return true;
}

void ThreadPool::shutdown()
{
	finished = true;
	printf("\nFinished\n\n");

	{
		cond.notify_all();
	}

	for (int i = 0; i < numOfWorkers; i++)
	{
		if (workers[i].joinable())
			workers[i].join();
	}
}