# AutoEstoque - API C++: testes separados do produto Qt.
# A aplicacao grafica e construida com qt/AutoEstoqueQt.pro (Qt Creator/qmake).
CXX ?= g++
CPPFLAGS ?= -Isrc
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
BIN := bin

ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif

FONTES_NEGOCIO := src/ServicoReposicao.cpp src/bib.cpp
OBJ_NEGOCIO := $(patsubst src/%.cpp,$(BIN)/%.o,$(FONTES_NEGOCIO))
OBJ_FUNCIONAL := $(BIN)/funcional_main.o $(BIN)/funcional_funcional_tests.o $(OBJ_NEGOCIO)
OBJ_UNIT := $(BIN)/unit_main.o $(BIN)/unit_unit_tests.o $(OBJ_NEGOCIO)

.PHONY: all test funcional unit qt clean
all: funcional unit
	@echo "Aplicativo Qt: abrir qt/AutoEstoqueQt.pro no Qt Creator para compilar/executar."

$(BIN):
	mkdir -p $(BIN)

$(BIN)/%.o: src/%.cpp src/ServicoReposicao.hpp src/bib.hpp | $(BIN)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BIN)/funcional_%.o: test/funcional/%.cpp test/funcional/funcional_tests.h src/ServicoReposicao.hpp | $(BIN)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BIN)/unit_%.o: test/unit/%.cpp test/unit/unit_tests.h src/ServicoReposicao.hpp | $(BIN)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BIN)/testeRegressivo$(EXE): $(OBJ_FUNCIONAL)
	$(CXX) $^ -o $@

$(BIN)/testeUnitario$(EXE): $(OBJ_UNIT)
	$(CXX) $^ -o $@

funcional: $(BIN)/testeRegressivo$(EXE)
	./$(BIN)/testeRegressivo$(EXE)

unit: $(BIN)/testeUnitario$(EXE)
	./$(BIN)/testeUnitario$(EXE)

test: funcional unit

qt:
	@echo "Abra qt/AutoEstoqueQt.pro no Qt Creator, selecione um Kit Desktop Qt e execute."

clean:
ifeq ($(OS),Windows_NT)
	-cmd /C "del /Q bin\\*.o bin\\testeRegressivo.exe bin\\testeUnitario.exe 2>nul"
else
	rm -f $(BIN)/*.o $(BIN)/testeRegressivo $(BIN)/testeUnitario
endif
