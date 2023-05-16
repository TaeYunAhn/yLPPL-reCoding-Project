#include <iostream>
#include <thread>
#include "ThreadPool.h"

// producer - consumer pattern
// threadpool

//const int PLUS_NUMBER = 1000;
//int num = 0;

//void plus()
//{
//	for (int i = 0; i < PLUS_NUMBER; i++) 
//	{
//		std::this_thread::sleep_for(std::chrono::milliseconds(10));
//		num++;
//	}
//}

int main()
{
	std::cout << "Hello World!\n";

	ThreadPool threadPool;
	threadPool.makeThread(4000);
	printf("\n");

	for (int i = 0; i < 100000000; i++)
	{
		threadPool.processJob(Job());
	}

	threadPool.shutdown();


	/*std::thread t1 = std::thread{ plus };
	std::thread t2 = std::thread{ plus };
	std::thread t3 = std::thread{ plus };
	std::thread t4 = std::thread{ plus };

	t1.join();
	t2.join();
	t3.join();
	t4.join();*/

	//std::cout << "num = " << num << std::endl;

}