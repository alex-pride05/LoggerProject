#include "Logger.h"
#include "ThreadSafety.h"
#include <iostream>
#include <thread>
#include <string>

struct LogMessage {
    std::string text;
    Loglevel level;
};

ThreadSafety<LogMessage> logQueue;

void logWorker(const std::string& filename, Loglevel defaultLevel) {
    Logger::init(filename, defaultLevel);
    LogMessage msg;
    while (true) {
        if (logQueue.pop(msg)) {
            Logger::write(msg.text, msg.level);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Использование: " << argv[0] << " <файл_журнала> <уровень>\n";
        return 1;
    }

    std::string filename = argv[1];
    Loglevel defaultLevel = static_cast<Loglevel>(std::stoi(argv[2]));

    std::thread worker(logWorker, filename, defaultLevel);
    worker.detach();

    std::string input;
    while (true) {
    std::cout << "Введите сообщение (или 'exit' для выхода): ";
    std::getline(std::cin, input);
    if (input == "exit") break;
    if (input.rfind("level ", 0) == 0) {
    	try {
            int newLevel = std::stoi(input.substr(6));
            if (newLevel >= 0 && newLevel <= 2) {
            	Loglevel levelEnum = static_cast<Loglevel>(newLevel);
                Logger::setLevel(levelEnum);
                defaultLevel = levelEnum;
            	std::cout << "Уровень логирования изменён на " << newLevel << "\n";
            }
	    else {
            	std::cout << "Неверный уровень. Используй 0=Debug, 1=Info, 2=Error\n";
            }
    	}
	catch (...) {
            std::cout << "Ошибка: неверный формат команды level N\n";
    	}
    	continue;
    }

    std::cout << "Введите уровень (0=Debug, 1=Info, 2=Error) или Enter для использования по умолчанию: ";
    std::string levelInput;
    std::getline(std::cin, levelInput);

    Loglevel msgLevel;

    bool validLevel = false;

    if (levelInput.empty()) {
   	validLevel = true;
   	msgLevel = defaultLevel;
    }
    else {
    	bool isNumber = true;
        for (char ch : levelInput) {
            if (!isdigit(ch)) {
            	isNumber = false;
            	break;
            }
        }

    	if (isNumber) {
            int levelVal = std::stoi(levelInput);
            if (levelVal >= 0 && levelVal <= 2) {
            	msgLevel = static_cast<Loglevel>(levelVal);
            	validLevel = true;
            }
	    else {
            	std::cout << "Недопустимый уровень. Сообщение не записано.\n";
            }
    	}
	else {
            std::cout << "Ошибка ввода уровня. Сообщение не записано.\n";
    	}
    }

    if (validLevel) {
    	logQueue.push({ input, msgLevel });
    }
    }
    return 0;
}
