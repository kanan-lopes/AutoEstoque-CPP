#include "unit_tests.h"
#include "ServicoReposicao.hpp"

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace autoestoque;

namespace {
void exigir(bool condicao, const char* mensagem) {
    if (!condicao) throw std::runtime_error(mensagem);
}
struct ConsultaStub final : IConsultaFiliais {
    std::vector<OfertaFilial> ofertas;
    std::vector<OfertaFilial> consultar(const std::string&) const override { return ofertas; }
};
struct EnvioStub final : IEnvioPedidos {
    int envios = 0;
    bool enviar(const Pedido&) override { ++envios; return true; }
};
Produto produto(int quantidade = 5) {
    return {"SSD480", "FORN01", quantidade, 5, 20};
}
} // namespace

void executarTestesUnitarios() {
    ConsultaStub consulta;
    EnvioStub envio;

    // U01: aceita configuracao valida.
    ServicoReposicao servico(consulta, envio, ConfiguracaoReposicao{10.0, 3});
    exigir(servico.consultarDisponibilidade(produto()).empty(), "U01 configuracao valida");

    // U02 e U03: rejeita parametros invalidos na construcao.
    bool rejeitou = false;
    try { ServicoReposicao invalido(consulta, envio, -1.0, 3); }
    catch (const std::invalid_argument&) { rejeitou = true; }
    exigir(rejeitou, "U02 distancia negativa");
    rejeitou = false;
    try { ServicoReposicao invalido(consulta, envio, 10.0, 0); }
    catch (const std::invalid_argument&) { rejeitou = true; }
    exigir(rejeitou, "U03 quantidade razoavel igual a zero");

    // U04: loja exatamente na distancia limite e elegivel.
    consulta.ofertas = {{"F1", 10.0, 3}};
    exigir(servico.consultarDisponibilidade(produto()).size() == 1, "U04 limite de distancia inclusivo");

    // U05: empate em distancia e ordenado por identificador da filial.
    consulta.ofertas = {{"B", 4.0, 5}, {"A", 4.0, 5}};
    const auto ordenadas = servico.consultarDisponibilidade(produto());
    exigir(ordenadas.size() == 2 && ordenadas[0].lojaId == "A", "U05 desempate por lojaId");

    // U06: oferta com distancia negativa e desconsiderada.
    consulta.ofertas = {{"F1", -1.0, 8}};
    exigir(servico.consultarDisponibilidade(produto()).empty(), "U06 distancia de oferta negativa");

    // U07: produto sem SKU e considerado invalido.
    const Produto semSku{"", "FORN01", 5, 5, 20};
    exigir(servico.consultarDisponibilidade(semSku).empty(), "U07 produto sem SKU");

    // U08: estoque acima do ponto nao permite sugestao.
    consulta.ofertas = {{"F1", 4.0, 8}};
    exigir(!servico.sugerirTransferencia(produto(6)).disponivel, "U08 sem sugestao acima do ponto");

    // U09: pedido com fornecedor ausente e rejeitado sem envio.
    const Produto semFornecedor{"SSD480", "", 5, 5, 20};
    const auto resultado = servico.gerarPedidoAoFornecedor(semFornecedor, "MATRIZ", false);
    exigir(resultado.status == StatusPedido::DadosInvalidos && envio.envios == 0,
           "U09 sem fornecedor nao emite pedido");
}
