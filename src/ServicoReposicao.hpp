/**
 * @file ServicoReposicao.hpp
 * @brief Contratos de domínio, interfaces e serviço de reposição de estoque.
 */

#pragma once

#include <string>
#include <vector>

namespace autoestoque {

struct Produto {
    std::string sku;
    std::string fornecedorId;
    int quantidadeAtual;
    int pontoPedido;
    int estoqueMaximo;
};

struct OfertaFilial {
    std::string lojaId;
    double distanciaKm;
    int quantidade;
};

struct SugestaoTransferencia {
    bool disponivel;
    std::string lojaId;
    int quantidade;
};

struct Pedido {
    std::string sku;
    std::string fornecedorId;
    std::string lojaSolicitante;
    int quantidade;
};

enum class StatusPedido {
    Gerado,
    AguardandoDecisao,
    EstoqueAdequado,
    DadosInvalidos,
    FalhaEnvio
};

struct ResultadoPedido {
    StatusPedido status;
    int quantidadeSolicitada;
};

struct ConfiguracaoReposicao {
    double distanciaMaxKm;
    int quantidadeRazoavel;
};

/**
 * @brief Porta de saída para consulta de estoque nas filiais da rede.
 */
class IConsultaFiliais {
public:
    virtual ~IConsultaFiliais() = default;
    virtual std::vector<OfertaFilial> consultar(const std::string& sku) const = 0;
};

/**
 * @brief Porta de saída para submissão de pedidos ao fornecedor padrão.
 */
class IEnvioPedidos {
public:
    virtual ~IEnvioPedidos() = default;
    virtual bool enviar(const Pedido& pedido) = 0;
};

/**
 * @brief Camada de lógica de negócio do Serviço de Reposição de Estoque (F01, F02 e F03).
 */
class ServicoReposicao {
public:
    ServicoReposicao(IConsultaFiliais& consulta, IEnvioPedidos& envio,
                     double distanciaMaxKm, int quantidadeRazoavel);
    ServicoReposicao(IConsultaFiliais& consulta, IEnvioPedidos& envio,
                     ConfiguracaoReposicao configuracao);

    /// F01: Retorna ofertas elegíveis dentro do raio e da quantidade mínima configurados.
    std::vector<OfertaFilial> consultarDisponibilidade(const Produto& produto) const;

    /// F02: Sugere transferência da filial mais próxima quando o produto atinge o ponto de pedido.
    SugestaoTransferencia sugerirTransferencia(const Produto& produto) const;

    /// F03: Gera pedido ao fornecedor quando não há transferência viável ou após recusa do operador.
    ResultadoPedido gerarPedidoAoFornecedor(const Produto& produto,
                                            const std::string& lojaSolicitante,
                                            bool transferenciaRecusada);

private:
    IConsultaFiliais& consulta_;
    IEnvioPedidos& envio_;
    ConfiguracaoReposicao configuracao_;

    bool produtoValido(const Produto& produto) const;
};

} // namespace autoestoque
