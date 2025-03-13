CXX = g++
CXXFLAGS = -std=c++11 -Iinclude
LDFLAGS = -Llib -ldxl_x64_cpp
SRC_DIR = src
BUILD_DIR = build

SRCS = $(SRC_DIR)/DynamixelManager.cpp $(SRC_DIR)/DynamixelAXControl.cpp $(SRC_DIR)/prueba.cpp
OBJS = $(BUILD_DIR)/DynamixelManager.o $(BUILD_DIR)/DynamixelAXControl.o $(BUILD_DIR)/prueba.o
TARGET = $(BUILD_DIR)/ejemplo

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean