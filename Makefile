# AutoEstoque - Sprint 03: produto, testes funcionais e testes unitarios
CXX ?= g++
CPPFLAGS ?= -Isrc
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
BIN := bin

ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif

FONTES_NEGOCIO := $(filter-out src/main.cpp,$(wildcard src/*.cpp))
OBJ_NEGOCIO := $(patsubst src/%.cpp,$(BIN)/%.o,$(FONTES_NEGOCIO))
OBJ_PRODUTO := $(BIN)/main.o $(OBJ_NEGOCIO)
OBJ_FUNCIONAL := $(BIN)/funcional_main.o $(BIN)/funcional_funcional_tests.o $(OBJ_NEGOCIO)
OBJ_UNIT := $(BIN)/unit_main.o $(BIN)/unit_unit_tests.o $(OBJ_NEGOCIO)

.PHONY: all test funcional unit clean
all: $(BIN)/autoestoque$(EXE)

$(BIN):
	mkdir -p $(BIN)

$(BIN)/%.o: src/%.cpp | $(BIN)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BIN)/funcional_%.o: test/funcional/%.cpp | $(BIN)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BIN)/unit_%.o: test/unit/%.cpp | $(BIN)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BIN)/autoestoque$(EXE): $(OBJ_PRODUTO)
	$(CXX) $^ -o $@

$(BIN)/testeRegressivo$(EXE): $(OBJ_FUNCIONAL)
	$(CXX) $^ -o $@

$(BIN)/testeUnitario$(EXE): $(OBJ_UNIT)
	$(CXX) $^ -o $@

funcional: $(BIN)/testeRegressivo$(EXE)
	./$(BIN)/testeRegressivo$(EXE)

unit: $(BIN)/testeUnitario$(EXE)
	./$(BIN)/testeUnitario$(EXE)

test: funcional unit

clean:
	rm -f $(BIN)/*.o $(BIN)/*.exe $(BIN)/autoestoque $(BIN)/testeRegressivo $(BIN)/testeUnitario
