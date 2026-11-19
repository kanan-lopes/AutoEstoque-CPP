#pragma once

#include "IServicoReposicao.hpp"

namespace autoestoque {

// Implementação concreta do Serviço de Reposição na Forma Canônica.
class ServicoReposicaoImpl final : public IServicoReposicao {
public:
    ServicoReposicaoImpl(IConsultaFiliais& consulta, IEnvioPedidos& envio,
                         double distanciaMaxKm, int quantidadeRazoavel);
    ServicoReposicaoImpl(IConsultaFiliais& consulta, IEnvioPedidos& envio,
                         ConfiguracaoReposicao configuracao);
    ~ServicoReposicaoImpl() override = default;

    std::vector<OfertaFilial> consultarDisponibilidade(const Produto& produto) const override;
    SugestaoTransferencia sugerirTransferencia(const Produto& produto) const override;
    ResultadoPedido gerarPedidoAoFornecedor(const Produto& produto,
                                            const std::string& lojaSolicitante,
                                            bool transferenciaRecusada) override;

private:
    // Forma Canônica: impede cópia e atribuição de objetos que mantêm referências internas.
    ServicoReposicaoImpl(const ServicoReposicaoImpl&) = delete;
    ServicoReposicaoImpl& operator=(const ServicoReposicaoImpl&) = delete;

    IConsultaFiliais& consulta_;
    IEnvioPedidos& envio_;
    ConfiguracaoReposicao configuracao_;

    bool produtoValido(const Produto& produto) const;
};

} // namespace autoestoque
