#include "Logger.h"
#include <ctime>
#include <chrono>
#include <vector>
#include <bitset>

std::vector<LogEntry> Logger::messages;

std::string Logger::getDateString() {
    time_t now = time(nullptr);
    tm timeinfo{};
    localtime_s(&timeinfo, &now);

    std::ostringstream timestream;
    timestream << std::put_time(&timeinfo, "%d-%m-%Y %H:%M:%S");
    return timestream.str();
}

void Logger::Log(const std::string &message) {
    if ((LOG_INFO & LOG_LEVEL) != LOG_LEVEL) {
        return;
    }
    auto s = getDateString();

    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.message =  "[" + s +  "] - " + message;

    messages.push_back(logEntry);
    std::cout << "[" << GREEN << s <<  RESET << "] - " << message << std::endl;

}


void Logger::Log(const int message) {
    if ((LOG_INFO & LOG_LEVEL) != LOG_LEVEL) {
        return;
    }
    auto s = getDateString();

    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.message =  "[" + s +  "] - " + std::to_string(message);

    messages.push_back(logEntry);
    std::cout << "[" << GREEN << s <<  RESET << "] - " << message << std::endl;

}


void Logger::Log(const float message) {
    if ((LOG_INFO & LOG_LEVEL) != LOG_LEVEL) {
        return;
    }
    auto s = getDateString();

    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.message =  "[" + s +  "] - " + std::to_string(message);

    messages.push_back(logEntry);
    std::cout << "[" << GREEN << s <<  RESET << "] - " << message << std::endl;

}



void Logger::Err(const std::string &message) {
    auto s = getDateString();
    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.message =  "[" +s +  "] - " + message;

    messages.push_back(logEntry);
    std::cerr << "[\x1B[91m" << s  <<  "\033[0m] - " << message << std::endl;
}

