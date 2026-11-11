#include "unit_tests.h"
#include <exception>
#include <iostream>

int main() {
    try {
        executarTestesUnitarios();
        std::cout << "PASS: 9 testes unitarios aprovados\n";
    } catch (const std::exception& erro) {
        std::cerr << "FALHA nos testes unitarios: " << erro.what() << '\n';
        return 1;
    }
    return 0;
}
