#pragma once

#include <memory>
#include <mutex>

using namespace std;

template <typename T>
class CSingletonTemplate
{
protected:
	static std::unique_ptr<T> instance;
	static mutex staticSingletonMx;

protected:
	CSingletonTemplate(){}
	virtual ~CSingletonTemplate(){}

public:
	static T * getInstance(){
		unique_lock<mutex> lock(staticSingletonMx);

		if (instance == nullptr)
			instance.reset(new T());

		return instance.get();
	};


	static void destroyInstance(){
		instance.reset(nullptr);
	};
};

template<typename T> std::unique_ptr<T> CSingletonTemplate<T>::instance(nullptr);
template<typename T> mutex CSingletonTemplate<T>::staticSingletonMx;