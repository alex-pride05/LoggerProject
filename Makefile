CCX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -fPIC
LDFLAGS = -shared
APP_LDFLAGS = -Lbuild -lLogger

SRC_LIB = LoggerLib/Logger.cpp
SRC_APP = LoggerApp/main.cpp

TARGET_LIB = build/libLogger.so
TARGET_APP = build/LoggerApp

.PHONY: all clean lib app

all: lib app

lib: $(TARGET_LIB)

$(TARGET_LIB): $(SRC_LIB)
	mkdir -p build
	$(CXX) $(CXXFLAGS) -I LoggerLib $(LDFLAGS) -o $@ $^

app: $(TARGET_APP)

$(TARGET_APP): $(SRC_APP)
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $^ $(APP_LDFLAGS) -I LoggerLib

clean:
	rm -rf build

