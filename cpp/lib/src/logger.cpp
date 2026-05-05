#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_sinks.h>

#include "quant_finance/logger.hpp"

using namespace qf;

logger& logger::get()
{
    static logger instance;
    return instance;
}

logger::logger() :
    m_logger(spdlog::stdout_logger_mt("quant_finance"))
{
    set_level(level::OFF);
    m_logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
}

void logger::set_level(const level log_level)
{
    auto spdlog_level = [log_level]() -> spdlog::level::level_enum
    {
        spdlog::level::level_enum spdlog_level;
        switch (log_level) // GCOVR_EXCL_BR_LINE - all enum values handled; implicit default is unreachable
        {
            case level::TRACE:
                spdlog_level = spdlog::level::level_enum::trace;
                break;
            case level::DEBUG:
                spdlog_level = spdlog::level::level_enum::debug;
                break;
            case level::INFO:
                spdlog_level = spdlog::level::level_enum::info;
                break;
            case level::WARN:
                spdlog_level = spdlog::level::level_enum::warn;
                break;
            case level::ERR:
                spdlog_level = spdlog::level::level_enum::err;
                break;
            case level::CRITICAL:
                spdlog_level = spdlog::level::level_enum::critical;
                break;
            case level::OFF:
                spdlog_level = spdlog::level::level_enum::off;
                break;
        }
        return spdlog_level;
    }();
    m_logger->set_level(spdlog_level);
    m_level = log_level;
}

logger::level logger::get_level() const
{
    return m_level;
}

void logger::trace(const std::string& msg) const
{
    m_logger->trace(msg);
}

void logger::debug(const std::string& msg) const
{
    m_logger->debug(msg);
}

void logger::info(const std::string& msg) const
{
    m_logger->info(msg);
}

void logger::warn(const std::string& msg) const
{
    m_logger->warn(msg);
}

void logger::error(const std::string& msg) const
{
    m_logger->error(msg);
}

void logger::critical(const std::string& msg) const
{
    m_logger->critical(msg);
}
