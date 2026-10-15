#include "bib.hpp"
#include <cassert>
#include <iostream>

int main() {
    // Feature 1: verificação do ponto de pedido
    assert(estoqueAtingiuPontoPedido(5, 5));
    assert(estoqueAtingiuPontoPedido(4, 5));
    assert(!estoqueAtingiuPontoPedido(6, 5));

    // Feature 2: cálculo da quantidade de reposição
    assert(calcularQuantidadeReposicao(4, 20) == 16);
    assert(calcularQuantidadeReposicao(20, 20) == 0);
    assert(calcularQuantidadeReposicao(22, 20) == 0);

    std::cout << "6 cenarios de teste aprovados.\n";
    return 0;
}
