#pragma once

#include <fstream>
#include <mutex>
#include <windows.h>

#include "SingletonTemplate.h"

enum eLogLevel {
	LogLevel_Debug,
	LogLevel_Info,
	LogLevel_Warning,
	LogLevel_Error,
	LogLevel_No
};

class CLogger : public CSingletonTemplate <CLogger>
{
private:
	mutex mx;
	string fileDate;
	ofstream logFile;
    string filePath;
    
public:
	CLogger();
	~CLogger();

	void out(const eLogLevel& aLogLevel, const thread::id& thread_id, const int& line, const char* func, LPCSTR msg, ...);
};



#define LOG_ERROR(logger, msg, ...) logger->out(eLogLevel::LogLevel_Error, std::this_thread::get_id(), __LINE__, __FUNCTION__, msg,  __VA_ARGS__)
#define LOG_DEBUG(logger, msg, ...) logger->out(eLogLevel::LogLevel_Debug, std::this_thread::get_id(), __LINE__, __FUNCTION__, msg,  __VA_ARGS__)
#define LOG_WARN(logger, msg, ...) logger->out(eLogLevel::LogLevel_Warning, std::this_thread::get_id(), __LINE__, __FUNCTION__, msg,  __VA_ARGS__)
#define LOG_INFO(logger, msg, ...) logger->out(eLogLevel::LogLevel_Info, std::this_thread::get_id(), __LINE__, __FUNCTION__, msg,  __VA_ARGS__)
