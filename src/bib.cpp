#include "bib.hpp"
#include <algorithm>

bool estoqueAtingiuPontoPedido(int quantidadeAtual, int pontoPedido) {
    return quantidadeAtual <= pontoPedido;
}

int calcularQuantidadeReposicao(int quantidadeAtual, int estoqueMaximo) {
    return std::max(0, estoqueMaximo - quantidadeAtual);
}
