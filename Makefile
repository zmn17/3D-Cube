	
# compiler and flags
CXX = g++
CXXFLAGS = -g -Wall -Wextra -std=c++17 -I/usr/include -I/usr/include/GLFW -Iexternal/include -Iexternal/include/imgui -Iexternal/include/imgui/backends
LDFLAGS = -L/usr/lib/ -lglfw -lGL -lm -lpthread -lX11 -ldl -Lexternal -lopengl_utils -limgui

# directories
SRC_DIR = src
BIN_DIR = bin
INCLUDE_DIR = include

# source and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp) external/src/glad.c 
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(filter %.cpp, $(SRCS))) \
	$(BIN_DIR)/glad.o

# executable name
EXEC = main

# default target
all: $(EXEC)

# linking
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# pattern rule for compiling .cpp to .o
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

#compile glad.c
$(BIN_DIR)/glad.o: external/src/glad.c | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# create bin dir if not exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# run the executable
run: all
	./$(EXEC)

# clean build artifacts
clean:
	rm -f $(BIN_DIR)/*.o $(EXEC)
