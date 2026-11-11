#include "ServicoReposicao.hpp"

#include <iostream>
#include <string>
#include <vector>

using namespace autoestoque;

// Demonstracao local: as integracoes externas sao simuladas.
class ConsultaDemonstracao final : public IConsultaFiliais {
public:
    std::vector<OfertaFilial> consultar(const std::string&) const override {
        return {{"FILIAL-A", 4.0, 8}, {"FILIAL-B", 12.0, 9}};
    }
};

class EnvioDemonstracao final : public IEnvioPedidos {
public:
    bool enviar(const Pedido& pedido) override {
        std::cout << "[SIMULACAO] Pedido: SKU=" << pedido.sku
                  << ", unidades=" << pedido.quantidade
                  << ", fornecedor=" << pedido.fornecedorId << '\n';
        return true;
    }
};

int main() {
    ConsultaDemonstracao consulta;
    EnvioDemonstracao envio;
    ServicoReposicao servico(consulta, envio, ConfiguracaoReposicao{10.0, 3});
    const Produto produto{"SSD480", "FORN01", 5, 5, 20};

    std::cout << "AutoEstoque - Sprint 03 (API de Reposicao)\n";

    // F01 - Consulta de ofertas validas nas filiais proximas.
    const auto ofertas = servico.consultarDisponibilidade(produto);
    std::cout << "F01: ofertas elegiveis = " << ofertas.size() << '\n';

    // F02 - Sugestao de transferencia.
    const auto sugestao = servico.sugerirTransferencia(produto);
    std::cout << "F02: transferencia = "
              << (sugestao.disponivel ? sugestao.lojaId : "indisponivel") << '\n';

    // F03 - Sem recusa, deve aguardar decisao do operador.
    const auto resultado = servico.gerarPedidoAoFornecedor(produto, "MATRIZ", false);
    std::cout << "F03: status = "
              << (resultado.status == StatusPedido::AguardandoDecisao
                      ? "aguardando decisao" : "outro status") << '\n';
    return 0;
}
