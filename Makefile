CXX = g++
CXXFLAGS = -std=c++17 -pthread -g -Wall
TARGET = simulador_visual_N

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp -lncurses

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET) config.txt

