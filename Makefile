CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

TARGET = systolic_model
SRCS = src/main.cpp

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
