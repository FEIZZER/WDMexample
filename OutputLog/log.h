#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"

class F_spdlog {
public:

	F_spdlog(void) {}
	~F_spdlog(void) {}

	static F_spdlog* getInstance()
	{
		static F_spdlog f_log;
		return &f_log;
	}

	static bool initLog(std::string& logFilePath)
	{
		F_spdlog::getInstance()->f_file_logger = spdlog::basic_logger_mt<spdlog::async_factory>("test_logger", logFilePath);
		F_spdlog::getInstance()->f_file_logger->set_level(spdlog::level::trace);
		F_spdlog::getInstance()->f_file_logger->flush_on(spdlog::level::trace);
		return F_spdlog::getInstance()->f_file_logger != nullptr;
	}

	static bool uninitLog(){}

	template<typename... Args>
	static void info(std::string fmt, Args &&... args)
	{
		F_spdlog::getInstance()->f_file_logger->log(spdlog::level::info, fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void error(std::string fmt, Args &&... args)
	{
		F_spdlog::getInstance()->f_file_logger->error(fmt, std::forward<Args>(args)...);
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

