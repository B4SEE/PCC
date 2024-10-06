#pragma once
#include "logging/aixlog.h"

inline void switchLoggingMode(AixLog::Severity severity) {
    // auto sink_cout = std::make_shared<AixLog::SinkCout>(AixLog::Filter(severity));
    auto sink_file = std::make_shared<AixLog::SinkFile>(AixLog::Filter(severity), "../log/logfile.log");
    // AixLog::Log::init({sink_cout, sink_file});
    AixLog::Log::init({sink_file});
}

inline void initLogging() {
    switchLoggingMode(AixLog::Severity::trace);

    if (std::ifstream log_header_file("../resources/log_header.txt"); !log_header_file.is_open()) {
        LOG(ERROR) << "Failed to open log_header file, proceeding without it.";
    } else {
        std::string log_header((std::istreambuf_iterator<char>(log_header_file)), std::istreambuf_iterator<char>());
        std::cout << log_header;
    }
}