#include "funcional_tests.h"
#include "ServicoReposicao.hpp"

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace autoestoque;

namespace {
struct ConsultaFake final : IConsultaFiliais {
    std::vector<OfertaFilial> ofertas;
    ConsultaFake() = default;
    explicit ConsultaFake(std::vector<OfertaFilial> dados) : ofertas(std::move(dados)) {}
    std::vector<OfertaFilial> consultar(const std::string&) const override { return ofertas; }
};
struct EnvioFake final : IEnvioPedidos {
    bool deveEnviar = true;
    std::vector<Pedido> enviados;
    bool enviar(const Pedido& pedido) override {
        if (deveEnviar) enviados.push_back(pedido);
        return deveEnviar;
    }
};
Produto produto(int quantidade = 5) { return {"SSD480", "FORN01", quantidade, 5, 20}; }
void exigir(bool condicao, const char* mensagem) {
    if (!condicao) throw std::runtime_error(mensagem);
}
} // namespace

// F01: quatro cenarios: filial elegivel, fora do raio, quantidade insuficiente e ordenacao.
void consultarDisponibilidadeFuncionalTest() {
    {
        ConsultaFake c({{"FILIAL-A", 4.0, 8}}); EnvioFake e; ServicoReposicao s(c, e, 10, 3);
        const auto ofertas = s.consultarDisponibilidade(produto());
        exigir(ofertas.size() == 1 && ofertas[0].lojaId == "FILIAL-A", "F01-T01 filial elegivel");
    }
    {
        ConsultaFake c({{"FILIAL-B", 11.0, 10}}); EnvioFake e; ServicoReposicao s(c, e, 10, 3);
        exigir(s.consultarDisponibilidade(produto()).empty(), "F01-T02 distancia maxima");
    }
    {
        ConsultaFake c({{"FILIAL-C", 4.0, 2}}); EnvioFake e; ServicoReposicao s(c, e, 10, 3);
        exigir(s.consultarDisponibilidade(produto()).empty(), "F01-T03 quantidade razoavel");
    }
    {
        ConsultaFake c({{"FILIAL-B", 8.0, 10}, {"FILIAL-A", 3.0, 9}});
        EnvioFake e; ServicoReposicao s(c, e, 10, 3);
        const auto ofertas = s.consultarDisponibilidade(produto());
        exigir(ofertas.size() == 2 && ofertas.front().lojaId == "FILIAL-A", "F01-T04 ordenacao por proximidade");
    }
}

// F02: tres cenarios: no ponto de pedido, acima do ponto e sem filiais.
void sugerirTransferenciaFuncionalTest() {
    {
        ConsultaFake c({{"FILIAL-A", 4.0, 8}}); EnvioFake e; ServicoReposicao s(c, e, 10, 3);
        const auto sugestao = s.sugerirTransferencia(produto(5));
        exigir(sugestao.disponivel && sugestao.lojaId == "FILIAL-A", "F02-T01 sugestao elegivel");
    }
    {
        ConsultaFake c({{"FILIAL-A", 4.0, 8}}); EnvioFake e; ServicoReposicao s(c, e, 10, 3);
        exigir(!s.sugerirTransferencia(produto(6)).disponivel, "F02-T02 estoque acima do ponto");
    }
    {
        ConsultaFake c; EnvioFake e; ServicoReposicao s(c, e, 10, 3);
        exigir(!s.sugerirTransferencia(produto(5)).disponivel, "F02-T03 sem filial");
    }
}

// F03: seis cenarios: gerar, aguardar, recusar, estoque adequado, falha, dados invalidos.
void gerarPedidoAoFornecedorFuncionalTest() {
    {
        ConsultaFake c; EnvioFake e; ServicoReposicao s(c, e, 10, 3);
        const auto r = s.gerarPedidoAoFornecedor(produto(5), "MATRIZ", false);
        exigir(r.status == StatusPedido::Gerado && r.quantidadeSolicitada == 15 &&
               e.enviados.size() == 1 && e.enviados[0].fornecedorId == "FORN01", "F03-T01 gera pedido");
    }
    {
        ConsultaFake c({{"FILIAL-A", 4.0, 8}}); EnvioFake e; ServicoReposicao s(c, e, 10, 3);
        const auto r = s.gerarPedidoAoFornecedor(produto(5), "MATRIZ", false);
        exigir(r.status == StatusPedido::AguardandoDecisao && e.enviados.empty(), "F03-T02 aguarda decisao");
    }
    {
        ConsultaFake c({{"FILIAL-A", 4.0, 8}}); EnvioFake e; ServicoReposicao s(c, e, 10, 3);
        const auto r = s.gerarPedidoAoFornecedor(produto(5), "MATRIZ", true);
        exigir(r.status == StatusPedido::Gerado && e.enviados.size() == 1, "F03-T03 recusa permite pedido");
    }
    {
        ConsultaFake c; EnvioFake e; ServicoReposicao s(c, e, 10, 3);
        const auto r = s.gerarPedidoAoFornecedor(produto(6), "MATRIZ", false);
        exigir(r.status == StatusPedido::EstoqueAdequado && e.enviados.empty(), "F03-T04 estoque adequado");
    }
    {
        ConsultaFake c; EnvioFake e; e.deveEnviar = false; ServicoReposicao s(c, e, 10, 3);
        const auto r = s.gerarPedidoAoFornecedor(produto(5), "MATRIZ", false);
        exigir(r.status == StatusPedido::FalhaEnvio && e.enviados.empty(), "F03-T05 falha no envio");
    }
    {
        ConsultaFake c; EnvioFake e; ServicoReposicao s(c, e, 10, 3);
        const Produto invalido{"SSD480", "", 5, 5, 20};
        const auto r = s.gerarPedidoAoFornecedor(invalido, "MATRIZ", false);
        exigir(r.status == StatusPedido::DadosInvalidos && e.enviados.empty(), "F03-T06 dados invalidos");
    }
}
