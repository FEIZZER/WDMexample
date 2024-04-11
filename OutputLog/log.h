#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

class F_spdlog {
public:

	F_spdlog(void) {}
	~F_spdlog(void) {}

	static F_spdlog* getInstance()
	{
		static F_spdlog f_log;
		return &f_log;
	}

	static bool initLog(const std::string& logFilePath)
	{
		F_spdlog::getInstance()->f_file_logger = spdlog::basic_logger_mt<spdlog::async_factory>("test_logger", logFilePath);
		if (F_spdlog::getInstance()->f_file_logger == nullptr)
			return false;
		// 设置日志格式. 参数含义: [日志标识符] [日期] [日志级别] [线程号] [文件名:行号] [数据]
		F_spdlog::getInstance()->f_file_logger->set_pattern("[%n] [%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%s:%#]  %v");
		F_spdlog::getInstance()->f_file_logger->set_level(spdlog::level::trace);
		F_spdlog::getInstance()->f_file_logger->flush_on(spdlog::level::trace);
		return true;
	}

	static bool uninitLog(){}

	template<typename... Args>
	static void info(std::string fmt, Args &&... args)
	{
		SPDLOG_LOGGER_INFO(F_spdlog::getInstance()->f_file_logger, fmt, args...);
		// F_spdlog::getInstance()->f_file_logger->log(spdlog::level::info, fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void error(std::string fmt, Args &&... args)
	{
		SPDLOG_LOGGER_ERROR(F_spdlog::getInstance()->f_file_logger, fmt, args...);
		// F_spdlog::getInstance()->f_file_logger->error(fmt, std::forward<Args>(args)...);
	}

	static bool setTemporaryLogPath(const std::string& log_path)
	{
		F_spdlog::getInstance()->f_file_path = log_path;
		return true;
	}
	template<typename... Args>
	static void t_info(std::string fmt, Args &&... args)
	{
		auto temp_logger = 
			spdlog::basic_logger_mt<spdlog::async_factory>("temp_logger", F_spdlog::getInstance()->f_file_path);
		temp_logger->log(spdlog::level::info, fmt, std::forward<Args>(args)...);
		spdlog::drop("temp_logger");
	}

	template<typename... Args>
	static void t_error(std::string fmt, Args &&... args)
	{
		auto temp_logger = 
			spdlog::basic_logger_mt<spdlog::async_factory>("temp_logger", F_spdlog::getInstance()->f_file_path);
		temp_logger->log(spdlog::level::err, fmt, std::forward<Args>(args)...);
		spdlog::drop("temp_logger");
	}
private:
	std::shared_ptr<spdlog::logger> f_file_logger;
	std::string f_file_path;
};

