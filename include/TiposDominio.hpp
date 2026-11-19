#pragma once

#include <string>

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

} // namespace autoestoque
