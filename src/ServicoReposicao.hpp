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

// Porta de consulta: uma implementação real pode usar API entre filiais.
class IConsultaFiliais {
public:
    virtual ~IConsultaFiliais() = default;
    virtual std::vector<OfertaFilial> consultar(const std::string& sku) const = 0;
};

// Porta de envio: uma implementação real pode usar e-mail/serviço externo.
class IEnvioPedidos {
public:
    virtual ~IEnvioPedidos() = default;
    virtual bool enviar(const Pedido& pedido) = 0;
};

class ServicoReposicao {
public:
    ServicoReposicao(IConsultaFiliais& consulta, IEnvioPedidos& envio,
                     double distanciaMaxKm, int quantidadeRazoavel);
    ServicoReposicao(IConsultaFiliais& consulta, IEnvioPedidos& envio,
                     ConfiguracaoReposicao configuracao);

    // F01: retorna apenas filiais dentro da distância e da disponibilidade configuradas.
    std::vector<OfertaFilial> consultarDisponibilidade(const Produto& produto) const;

    // F02: sugere loja com estoque, caso o produto esteja no ponto de pedido.
    SugestaoTransferencia sugerirTransferencia(const Produto& produto) const;

    // F03: gera pedido se necessário e se não houver transferência pendente de decisão.
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
