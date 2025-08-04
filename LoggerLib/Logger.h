#pragma once
#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <filesystem>
#include <mutex>

enum class Loglevel { Debug, Information, Error };

class Logger {
public:
	static void init(const std::string& filename, Loglevel level);
	static void write(const std::string& message, Loglevel level);
	static void setLevel(Loglevel newlevel);
private:
	static std::ofstream logFile;
	static Loglevel currentLevel;
	static std::mutex logMutex;
	static std::string levelToString(Loglevel level);
};
