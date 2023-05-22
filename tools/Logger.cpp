//+------------------------------------------------------------------+
//|                                                  Trade Collector |
//|                 Copyright ?2005-2008, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+

#include "stdafx.h"

#include <sstream>
#include <fstream>
#include "logger.h"
#include "Settings.h"

using namespace std;

CLogger::CLogger()
{
}

CLogger::~CLogger()
{
	//out(LogLevel_Info, this_thread::get_id(), __LINE__, __FUNCTION__, "");
	unique_lock<mutex> lock(mx);

	if (logFile.is_open()) {
		logFile.flush();
		logFile.close();
	}

}

void CLogger::out(const eLogLevel& eLogLevel, const thread::id& thread_id, const int& line, const char* func, LPCSTR msg, ...)
{
	if (eLogLevel < CSettings::getInstance()->getLogLevel())
		return;

	try {
		char strBuf[SHRT_MAX];
		memset(strBuf, 0, sizeof(strBuf));
		char       tmp[1024];
		memset(tmp, 0, sizeof(tmp));
		char currentDate[12];
		memset(currentDate, 0, sizeof(currentDate));
		SYSTEMTIME st;
		va_list    arg_ptr;

		//---- check
		if (msg == NULL) return;
		//---- take current time
		GetLocalTime(&st);
		sprintf_s(currentDate, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);

		unique_lock<mutex> lock(mx);

		if (fileDate != currentDate) {
			fileDate = currentDate;

			if (logFile.is_open())
				logFile.close();

			_snprintf_s(tmp, sizeof(tmp) - 1, "%s\\logs\\LPPL.%04d%02d%02d.log", filePath.c_str(), st.wYear, st.wMonth, st.wDay);
			logFile.open(tmp, std::ofstream::out | std::ofstream::app);
		}

		//----
		va_start(arg_ptr, msg);
		int len = _vscprintf(msg, arg_ptr);
		vsprintf_s(strBuf, len + 1, msg, arg_ptr);
		va_end(arg_ptr);

		//---- write into log
		if (logFile.is_open())
		{
			string logLevelHeader = "";
			switch (eLogLevel)
			{
			case LogLevel_Info:
				logLevelHeader = "[INFO]"; break;
			case LogLevel_Debug:
				logLevelHeader = "[DEBUG]"; break;
			case LogLevel_Warning:
				logLevelHeader = "[WARNING]"; break;
			case LogLevel_Error:
				logLevelHeader = "[ERROR]"; break;
			}

			stringstream ss;
			ss << thread_id;

			char strOutBuf[SHRT_MAX];
			sprintf_s(strOutBuf, "%s\t%04d.%02d.%02d %02d:%02d:%02d.%03d  [%s]%s()[%d] %s",
				logLevelHeader.c_str(), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, ss.str().c_str(), func, line, strBuf);

			logFile << strOutBuf << endl;
			logFile.flush();
		}
	}
	catch (exception e) {
		SYSTEMTIME st;
		GetLocalTime(&st);
		char       tmp[1024];
		memset(tmp, 0, sizeof(tmp));
		_snprintf_s(tmp, sizeof(tmp) - 1, "%s\\logs\\Error.%04d%02d%02d.log", FILE_PATH, st.wYear, st.wMonth, st.wDay);
		char strOutBuf[4096];
		sprintf_s(strOutBuf, "%04d.%02d.%02d %02d:%02d:%02d.%03d  Error occurred(%s)",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, e.what());

		auto fd = fopen(tmp, "w");
		fprintf(fd, "%s", strOutBuf);
		fclose(fd);
		return;
	}
}