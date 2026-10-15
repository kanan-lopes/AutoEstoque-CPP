CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
CPPFLAGS ?= -Isrc -Iinclude
DEPFLAGS := -MMD -MP
BIN := bin
TARGET := $(BIN)/autoestoque
TEST_TARGET := $(BIN)/testeRegressivo
SOURCES := $(wildcard src/*.cpp)
APP_OBJECTS := $(patsubst src/%.cpp,$(BIN)/%.o,$(SOURCES))
LIB_SOURCES := $(filter-out src/main.cpp,$(SOURCES))
LIB_OBJECTS := $(patsubst src/%.cpp,$(BIN)/%.o,$(LIB_SOURCES))
TEST_SOURCES := $(wildcard test/*.cpp)
TEST_OBJECTS := $(patsubst test/%.cpp,$(BIN)/test_%.o,$(TEST_SOURCES))

ifeq ($(OS),Windows_NT)
EXE := .exe
RM_BIN := cmd /C del /Q
else
EXE :=
RM_BIN := rm -f
endif

-include $(APP_OBJECTS:.o=.d) $(TEST_OBJECTS:.o=.d)

.PHONY: all test clean
all: $(TARGET)$(EXE)

$(BIN):
	mkdir -p $(BIN)

$(BIN)/%.o: src/%.cpp | $(BIN)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

$(BIN)/test_%.o: test/%.cpp | $(BIN)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

$(TARGET)$(EXE): $(APP_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_TARGET)$(EXE): $(LIB_OBJECTS) $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

test: $(TEST_TARGET)$(EXE)
	./$(TEST_TARGET)$(EXE)

clean:
	$(RM_BIN) $(BIN)/*.o $(BIN)/*.d $(TARGET)$(EXE) $(TEST_TARGET)$(EXE)
