#ifndef AUTOESTOQUE_BIB_HPP
#define AUTOESTOQUE_BIB_HPP

// Retorna true quando a quantidade atual atinge ou fica abaixo do ponto de pedido.
bool estoqueAtingiuPontoPedido(int quantidadeAtual, int pontoPedido);

// Retorna a quantidade para chegar ao estoque maximo, nunca negativa.
int calcularQuantidadeReposicao(int quantidadeAtual, int estoqueMaximo);

#endif
