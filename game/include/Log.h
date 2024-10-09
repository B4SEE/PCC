#pragma once
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include <fstream>

inline std::shared_ptr<spdlog::logger> logger;

#define LOG_TRACE(...) logger->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::trace, __VA_ARGS__)
#define LOG_INFO(...) logger->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::info, __VA_ARGS__)
#define LOG_DEBUG(...) logger->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::debug, __VA_ARGS__)
#define LOG_ERROR(...) logger->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::err, __VA_ARGS__)
#define LOG_WARN(...) logger->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::warn, __VA_ARGS__)
#define LOG_CRITICAL(...) logger->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::critical, __VA_ARGS__)

inline void initFileLogger() {
    try
    {
        std::ofstream ofs;
        ofs.open("../log/logfile.log", std::ofstream::out | std::ofstream::trunc);
        ofs.close();

        logger = spdlog::basic_logger_mt("basic_logger", "../log/logfile.log");
        logger->set_pattern("[%Y-%m-%d %H:%M:%S] [thread %-5t] %-8l | %15s:%5# - %v");

        if (std::ifstream log_header_file("../resources/log_header.txt"); !log_header_file.is_open()) {
            LOG_WARN("Failed to open log header file.");
        } else {
            std::string log_header((std::istreambuf_iterator<char>(log_header_file)), std::istreambuf_iterator<char>());
            LOG_INFO(log_header);
        }

        logger->flush_on(spdlog::level::trace);
    }
    catch (const spdlog::spdlog_ex &ex)
    {
        throw std::runtime_error("Log initialization failed: " + std::string(ex.what()));
    }
}