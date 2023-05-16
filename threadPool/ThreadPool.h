#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <queue>

struct Job
{
	unsigned int id;
	std::string symbol;
	int side;
	int orderType;
	double price;
	double qty;
};


class ThreadPool
{
public:
	ThreadPool() : finished(false) {}
	~ThreadPool() {}

	void makeThread(const int th_num);	// init
	void processJob(const Job& job);	// producer
	void shutdown();

private:
	void work(int idx);	// consumer
	void sendJob(const Job& job);	// producer
	bool doJob(const Job& job);


private:
	std::vector<std::thread> workers;
	std::queue<Job> jobQueue;
	std::mutex mx;
	std::condition_variable cond;

	std::atomic<bool> finished;
	int numOfWorkers;
};

