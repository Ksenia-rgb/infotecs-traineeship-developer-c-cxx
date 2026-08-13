.PHONY: all $(LIB) $(DEMO) $(TESTS) run test clean

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

LIB = tecslog
LIBRARY = $(BIN_DIR)/lib$@.so
DEMO = demo-app
TESTS = tests

CXX = g++
CPPFLAGS = -Wall -Wextra -fPIC -I$(LIB)/include -I$(LIB)/src -I$(DEMO) -I$(TESTS)
CXXFLAGS = -g
LDFLAGS = -L$(BIN_DIR) -l$(LIB)

system   := $(shell uname)
ifneq 'MINGW' '$(patsubst MINGW%,MINGW,$(system))'
CPPFLAGS += -std=c++17
else
CPPFLAGS += -std=gnu++17
endif

tecslog_sources = $(wildcard $(LIB)/src/*.cpp)
tecslog_objects = $(addprefix $(OBJ_DIR)/, $(notdir $(tecslog_sources:.cpp=.o)))

demo_sources = $(wildcard $(DEMO)/*.cpp)
demo_objects = $(addprefix $(OBJ_DIR)/, $(notdir $(demo_sources:.cpp=.o)))

test_sources = $(wildcard $(TESTS)/*.cpp)
test_objects = $(addprefix $(OBJ_DIR)/, $(notdir $(test_sources:.cpp=.o)))

vpath %.cpp $(LIB)/src
vpath %.cpp $(DEMO)
vpath %.cpp $(TESTS)

all: $(LIB) $(DEMO) $(TESTS)

$(LIB): $(tecslog_objects) | $(BIN_DIR)
	@$(CXX) -shared -o $(LIBRARY) $^
	@echo "[LINK] $(LIBRARY)"

$(DEMO): $(demo_objects) $(LIB) | $(BIN_DIR)
	@$(CXX) -o $(BIN_DIR)/$@ $(demo_objects) $(LDFLAGS)
	@echo "[LINK] $(DEMO)"

$(TESTS): $(test_objects) $(LIB) | $(BIN_DIR)
	@$(CXX) -o $(BIN_DIR)/$@ $(test_objects) $(LDFLAGS)
	@echo "[LINK] $(TESTS)"

run: $(DEMO)
	@echo "[RUN] $(DEMO)"
	@LD_LIBRARY_PATH=$(BIN_DIR) ./$(BIN_DIR)/$(DEMO) $(ARGS)

test: $(TESTS)
	@echo "[RUN] $(TESTS)"
	@LD_LIBRARY_PATH=$(BIN_DIR) ./$(BIN_DIR)/$(TESTS)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
	@echo "[BUILD] $<"

$(BUILD_DIR):
	@mkdir -p $@

$(OBJ_DIR): $(BUILD_DIR)
	@mkdir -p $@

$(BIN_DIR): $(BUILD_DIR)
	@mkdir -p $@
