CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude
TARGET = university_system.exe
SOURCES = $(wildcard src/*.cpp) main.cpp

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	ifeq ($(OS),Windows_NT)
		del /Q $(TARGET)
	else
		rm -f $(TARGET)
	endif
