#pragma once

#include <memory>
#include <string>
#include <vector>
#include "TiposDominio.hpp"
#include "IConsultaFiliais.hpp"
#include "IEnvioPedidos.hpp"

namespace autoestoque {

// Interface pública da camada de Lógica de Negócios (F01, F02 e F03).
class IServicoReposicao {
public:
    virtual ~IServicoReposicao() = default;

    // Factory Method: cria instância concreta sem expor ServicoReposicaoImpl aos clientes.
    static std::unique_ptr<IServicoReposicao> criar(IConsultaFiliais& consulta,
                                                    IEnvioPedidos& envio,
                                                    double distanciaMaxKm,
                                                    int quantidadeRazoavel);

    static std::unique_ptr<IServicoReposicao> criar(IConsultaFiliais& consulta,
                                                    IEnvioPedidos& envio,
                                                    ConfiguracaoReposicao configuracao);

    // F01: retorna apenas filiais dentro da distância e da disponibilidade configuradas.
    virtual std::vector<OfertaFilial> consultarDisponibilidade(const Produto& produto) const = 0;

    // F02: sugere loja com estoque, caso o produto esteja no ponto de pedido.
    virtual SugestaoTransferencia sugerirTransferencia(const Produto& produto) const = 0;

    // F03: gera pedido ao fornecedor se necessário e sem transferência pendente de decisão.
    virtual ResultadoPedido gerarPedidoAoFornecedor(const Produto& produto,
                                                    const std::string& lojaSolicitante,
                                                    bool transferenciaRecusada) = 0;
};

} // namespace autoestoque
