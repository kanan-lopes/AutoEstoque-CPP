#include "ServicoReposicao.hpp"

#include <algorithm>
#include <stdexcept>

namespace autoestoque {

ServicoReposicao::ServicoReposicao(IConsultaFiliais& consulta, IEnvioPedidos& envio,
                                   double distanciaMaxKm, int quantidadeRazoavel)
    : ServicoReposicao(consulta, envio, ConfiguracaoReposicao{distanciaMaxKm, quantidadeRazoavel}) {}

ServicoReposicao::ServicoReposicao(IConsultaFiliais& consulta, IEnvioPedidos& envio,
                                   ConfiguracaoReposicao configuracao)
    : consulta_(consulta), envio_(envio), configuracao_(configuracao) {
    if (configuracao_.distanciaMaxKm < 0 || configuracao_.quantidadeRazoavel <= 0) {
        throw std::invalid_argument("Parâmetros de reposição inválidos");
    }
}

bool ServicoReposicao::produtoValido(const Produto& p) const {
    return !p.sku.empty() && !p.fornecedorId.empty() &&
           p.quantidadeAtual >= 0 && p.pontoPedido >= 0 &&
           p.estoqueMaximo >= p.pontoPedido;
}

std::vector<OfertaFilial>
ServicoReposicao::consultarDisponibilidade(const Produto& produto) const {
    if (!produtoValido(produto)) return {};
    std::vector<OfertaFilial> ofertas;
    for (const auto& oferta : consulta_.consultar(produto.sku)) {
        if (oferta.distanciaKm >= 0 && oferta.distanciaKm <= configuracao_.distanciaMaxKm &&
            oferta.quantidade >= configuracao_.quantidadeRazoavel) {
            ofertas.push_back(oferta);
        }
    }
    std::sort(ofertas.begin(), ofertas.end(), [](const auto& a, const auto& b) {
        if (a.distanciaKm != b.distanciaKm) return a.distanciaKm < b.distanciaKm;
        return a.lojaId < b.lojaId;
    });
    return ofertas;
}

SugestaoTransferencia
ServicoReposicao::sugerirTransferencia(const Produto& produto) const {
    if (!produtoValido(produto) || produto.quantidadeAtual > produto.pontoPedido) {
        return {false, "", 0};
    }
    const auto ofertas = consultarDisponibilidade(produto);
    if (ofertas.empty()) return {false, "", 0};
    return {true, ofertas.front().lojaId, ofertas.front().quantidade};
}

ResultadoPedido
ServicoReposicao::gerarPedidoAoFornecedor(const Produto& produto,
                                           const std::string& lojaSolicitante,
                                           bool transferenciaRecusada) {
    if (!produtoValido(produto) || lojaSolicitante.empty()) {
        return {StatusPedido::DadosInvalidos, 0};
    }
    if (produto.quantidadeAtual > produto.pontoPedido) {
        return {StatusPedido::EstoqueAdequado, 0};
    }
    if (sugerirTransferencia(produto).disponivel && !transferenciaRecusada) {
        return {StatusPedido::AguardandoDecisao, 0};
    }
    const int quantidade = produto.estoqueMaximo - produto.quantidadeAtual;
    if (quantidade <= 0) return {StatusPedido::DadosInvalidos, 0};
    const Pedido pedido{produto.sku, produto.fornecedorId, lojaSolicitante, quantidade};
    if (!envio_.enviar(pedido)) return {StatusPedido::FalhaEnvio, quantidade};
    return {StatusPedido::Gerado, quantidade};
}

} // namespace autoestoque
