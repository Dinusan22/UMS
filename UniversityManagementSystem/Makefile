CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude
TARGET = university_system.exe
SOURCES = $(wildcard src/*.cpp) main.cpp

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	$(RM) $(TARGET)
