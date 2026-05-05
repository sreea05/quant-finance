#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include "quant_finance/logger.hpp"

using qf::logger;

static void flush_spdlog()
{
    spdlog::get("quant_finance")->flush();
}

TEST(logger, singleton_returns_same_instance)
{
    logger& a = logger::get();
    logger& b = logger::get();
    EXPECT_EQ(&a, &b);
}

TEST(logger, set_and_get_level_trace)
{
    logger::get().set_level(logger::level::TRACE);
    EXPECT_EQ(logger::get().get_level(), logger::level::TRACE);
}

TEST(logger, set_and_get_level_debug)
{
    logger::get().set_level(logger::level::DEBUG);
    EXPECT_EQ(logger::get().get_level(), logger::level::DEBUG);
}

TEST(logger, set_and_get_level_info)
{
    logger::get().set_level(logger::level::INFO);
    EXPECT_EQ(logger::get().get_level(), logger::level::INFO);
}

TEST(logger, set_and_get_level_warn)
{
    logger::get().set_level(logger::level::WARN);
    EXPECT_EQ(logger::get().get_level(), logger::level::WARN);
}

TEST(logger, set_and_get_level_err)
{
    logger::get().set_level(logger::level::ERR);
    EXPECT_EQ(logger::get().get_level(), logger::level::ERR);
}

TEST(logger, set_and_get_level_critical)
{
    logger::get().set_level(logger::level::CRITICAL);
    EXPECT_EQ(logger::get().get_level(), logger::level::CRITICAL);
}

TEST(logger, set_and_get_level_off)
{
    logger::get().set_level(logger::level::OFF);
    EXPECT_EQ(logger::get().get_level(), logger::level::OFF);
}

TEST(logger, log_methods_do_not_throw)
{
    for (const auto lvl : {logger::level::TRACE, logger::level::OFF})
    {
        logger::get().set_level(lvl);
        EXPECT_NO_THROW(logger::get().trace("trace message"));
        EXPECT_NO_THROW(logger::get().debug("debug message"));
        EXPECT_NO_THROW(logger::get().info("info message"));
        EXPECT_NO_THROW(logger::get().warn("warn message"));
        EXPECT_NO_THROW(logger::get().error("error message"));
        EXPECT_NO_THROW(logger::get().critical("critical message"));
    }
    logger::get().set_level(logger::level::OFF);
}

TEST(logger, each_log_method_emits_its_message)
{
    logger::get().set_level(logger::level::TRACE);

    const struct
    {
        void (logger::*method)(const std::string&) const;
        std::string msg;
    } cases[] = {
        {   &logger::trace,    "trace output"},
        {   &logger::debug,    "debug output"},
        {    &logger::info,     "info output"},
        {    &logger::warn,     "warn output"},
        {   &logger::error,    "error output"},
        {&logger::critical, "critical output"},
    };

    for (const auto& c : cases)
    {
        testing::internal::CaptureStdout();
        (logger::get().*c.method)(c.msg);
        flush_spdlog();
        const std::string output = testing::internal::GetCapturedStdout();
        EXPECT_NE(output.find(c.msg), std::string::npos) << "Message not found: " << c.msg;
    }
    logger::get().set_level(logger::level::OFF);
}

TEST(logger, messages_below_active_level_are_suppressed)
{
    logger::get().set_level(logger::level::ERR);
    testing::internal::CaptureStdout();
    logger::get().info("should not appear");
    logger::get().debug("also hidden");
    flush_spdlog();
    const std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output.find("should not appear"), std::string::npos);
    EXPECT_EQ(output.find("also hidden"), std::string::npos);
    logger::get().set_level(logger::level::OFF);
}

TEST(logger, only_messages_at_or_above_level_appear)
{
    logger::get().set_level(logger::level::WARN);
    testing::internal::CaptureStdout();
    logger::get().debug("hidden debug");
    logger::get().info("hidden info");
    logger::get().warn("visible warn");
    logger::get().error("visible error");
    flush_spdlog();
    const std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output.find("hidden debug"), std::string::npos);
    EXPECT_EQ(output.find("hidden info"), std::string::npos);
    EXPECT_NE(output.find("visible warn"), std::string::npos);
    EXPECT_NE(output.find("visible error"), std::string::npos);
    logger::get().set_level(logger::level::OFF);
}
