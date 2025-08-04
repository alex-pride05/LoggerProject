#include "Logger.h"
#include <chrono>
#include <iomanip>
#include <iostream>

std::ofstream Logger::logFile;
Loglevel Logger::currentLevel = Loglevel::Information;
std::mutex Logger::logMutex;

void Logger::init(const std::string& filename, Loglevel level) {
    bool fileExists = false;
    try {
    	fileExists = std::filesystem::exists(filename);
    }
    catch (const std::filesystem::filesystem_error&) {
    	fileExists = false;
}
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
    std::cerr << "Ошибка: не удалось открыть файл лога: " << filename << "\n";
    std::exit(1);
    }
    currentLevel = level;
    if (!fileExists) {
        logFile << "\xEF\xBB\xBF";
    }
}
void Logger::write(const std::string& message, Loglevel level) {
    if (level < currentLevel) return;

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::lock_guard<std::mutex> lock(logMutex);

    std::tm tmBuf;
#ifdef _WIN32
    localtime_s(&tmBuf, &time);
#else
    localtime_r(&time, &tmBuf);
#endif

    logFile << "[" << std::put_time(&tmBuf, "%H:%M:%S") << "] "
        << "[" << levelToString(level) << "] "
        << message << '\n';
}

void Logger::setLevel(Loglevel newLevel) {
    currentLevel = newLevel;
}

std::string Logger::levelToString(Loglevel level) {
    switch (level) {
    case Loglevel::Debug: return "DEBUG";
    case Loglevel::Information:  return "INFO";
    case Loglevel::Error: return "ERROR";
    default:              return "UNKNOWN";
    }
}
