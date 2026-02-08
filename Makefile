# Makefile for C++ Snake Game with SFML

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
TARGET = snake_game

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	$(CXX) -g $(CXXFLAGS) main.cpp -o $(TARGET)-debug $(LDFLAGS)

.PHONY: all clean run debug
