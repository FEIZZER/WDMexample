#pragma once

#define SPDLOG_WCHAR_TO_UTF8_SUPPORT

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

class fspdlog {
public:

	fspdlog(void) {}
	~fspdlog(void) {}

	static fspdlog* getInstance()
	{
		static fspdlog f_log;
		return &f_log;
	}

	static bool initLog(const std::string& logName, const std::string& logFilePath)
	{
		// 不能使用 异步日志async_factory, 会导致UnHookWindows后, hook_dll无法被IM进程卸载
		// todo
		try
		{
			// fspdlog::getInstance()->f_file_logger = spdlog::rotating_logger_mt<spdlog::synchronous_factory>(logName, logFilePath, 1024 * 1024 * 5, 3);
			fspdlog::getInstance()->f_file_logger = 
				spdlog::rotating_logger_mt<spdlog::async_factory>(logName, logFilePath, 1024 * 1024 * 5, 3);
		}
		catch (...)
		{
		}

		if (fspdlog::getInstance()->f_file_logger == nullptr)
			return false;

		// 设置日志格式. 参数含义: [日志标识符] [日期] [日志级别] [进程号] [数据] [文件名:行号] 
		fspdlog::getInstance()->f_file_logger->set_pattern("%n [%Y-%m-%d %H:%M:%S] [%l] [pid:%P] [funcName:%!] %v [%s:%#]");
		fspdlog::getInstance()->f_file_logger->set_level(spdlog::level::trace);
		fspdlog::getInstance()->f_file_logger->flush_on(spdlog::level::trace);
		return true;
	}

	static bool uninitLog()
	{
		// spdlog::shutdown();
		spdlog::drop_all();
		fspdlog::getInstance()->f_file_logger.reset();
		return true;
	}

	static bool isInit()
	{
		return fspdlog::getInstance()->f_file_logger != NULL;
	}

	template<typename... Args>
	static void info(std::string fmt, Args &&... args)
	{
		//SPDLOG_LOGGER_INFO(F_spdlog::getInstance()->f_file_logger, fmt, args...);
		fspdlog::getInstance()->f_file_logger->log(spdlog::level::info, fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void error(std::string fmt, Args &&... args)
	{
		// SPDLOG_LOGGER_ERROR(F_spdlog::getInstance()->f_file_logger, fmt, args...);
		fspdlog::getInstance()->f_file_logger->error(fmt, std::forward<Args>(args)...);
	}

public:
	std::shared_ptr<spdlog::logger> f_file_logger;

private:
	std::string f_file_path;
};

#define LOG_INIT(var1, var2)	fspdlog::initLog(var1, var2);
#define LOG_UNINIT()			fspdlog::uninitLog();

#define TRACE_LOG(...)			if (fspdlog::getInstance()->f_file_logger != nullptr) SPDLOG_LOGGER_TRACE(fspdlog::getInstance()->f_file_logger, __VA_ARGS__)

#define DEBUG_LOG(...)			if (fspdlog::getInstance()->f_file_logger != nullptr) SPDLOG_LOGGER_DEBUG(fspdlog::getInstance()->f_file_logger, __VA_ARGS__)

#define INFO_LOG(...)			if (fspdlog::getInstance()->f_file_logger != nullptr) SPDLOG_LOGGER_INFO(fspdlog::getInstance()->f_file_logger, __VA_ARGS__)

#define WARN_LOG(...)			if (fspdlog::getInstance()->f_file_logger != nullptr) SPDLOG_LOGGER_WARN(fspdlog::getInstance()->f_file_logger, __VA_ARGS__)

#define ERROR_LOG(...)			if (fspdlog::getInstance()->f_file_logger != nullptr) SPDLOG_LOGGER_ERROR(fspdlog::getInstance()->f_file_logger, __VA_ARGS__)

#define CRITICAL_LOG(...)		if (fspdlog::getInstance()->f_file_logger != nullptr) SPDLOG_LOGGER_CRITICAL(fspdlog::getInstance()->f_file_logger, __VA_ARGS__)