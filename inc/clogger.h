#pragma once

#include <3thpart/spdlog/spdlog.h>
#include <3thpart/spdlog/sinks/basic_file_sink.h>


class clogger
{
public:
	static clogger& get_instance()
	{
		static clogger obj;
		return obj;
	}

	bool is_initialize() const
	{
		return  m_b_ini;
	}
	
	std::shared_ptr<spdlog::logger> get()
	{
		return m_logger;
	}
private:
	clogger() : m_b_ini(false)
	{
		try {
			m_logger = spdlog::basic_logger_mt("file_logger", "logs/basic-log.txt");
			m_logger->set_level(spdlog::level::trace);
			m_logger->set_pattern("[%s][%!][%#] %v");
			spdlog::set_default_logger(m_logger);

			m_b_ini = true;
		}
		catch (const spdlog::spdlog_ex& ex)
		{
		}

	}

private:
	std::shared_ptr<spdlog::logger> m_logger;
	bool m_b_ini;

private://don't call these methods
	clogger(const clogger&);
	clogger& operator=(const clogger&);
};