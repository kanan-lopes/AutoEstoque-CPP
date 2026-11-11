#include "funcional_tests.h"

#include <exception>
#include <iostream>

int main() {
    try {
        consultarDisponibilidadeFuncionalTest();
        std::cout << "PASS F01 - Consultar estoque em loja proxima\n";
        sugerirTransferenciaFuncionalTest();
        std::cout << "PASS F02 - Sugerir transferencia\n";
        gerarPedidoAoFornecedorFuncionalTest();
        std::cout << "PASS F03 - Gerar pedido ao fornecedor\n";
    } catch (const std::exception& erro) {
        std::cerr << "FALHA nos testes funcionais: " << erro.what() << '\n';
        return 1;
    }
    std::cout << "TOTAL: 3/3 testes funcionais aprovados (13 cenarios internos)\n";
    return 0;
}
