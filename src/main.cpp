#include "bib.hpp"
#include <iostream>

int main() {
    const int quantidadeAtual = 4;
    const int pontoPedido = 5;
    const int estoqueMaximo = 20;

    std::cout << "AutoEstoque - demonstracao inicial\n";
    std::cout << "Quantidade: " << quantidadeAtual << " | Ponto de pedido: " << pontoPedido << '\n';
    if (estoqueAtingiuPontoPedido(quantidadeAtual, pontoPedido)) {
        std::cout << "Reposicao necessaria. Quantidade sugerida: "
                  << calcularQuantidadeReposicao(quantidadeAtual, estoqueMaximo) << '\n';
    } else {
        std::cout << "Estoque acima do ponto de pedido.\n";
    }
    return 0;
}
