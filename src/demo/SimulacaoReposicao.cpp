#include "SimulacaoReposicao.hpp"

namespace autoestoque {

std::vector<OfertaFilial> ConsultaFiliaisDemo::consultar(const std::string& sku) const {
    if (sku != "SSD480") {
        return {}; // Catálogo demonstrativo contém apenas o SSD 480 GB.
    }
    // A primeira filial é elegível com configuração padrão (10 km, 3 un.).
    // FILIAL-B está fora do raio; FILIAL-C tem quantidade insuficiente.
    return {
        {"FILIAL-B", 12.0, 9},
        {"FILIAL-A", 4.0, 8},
        {"FILIAL-C", 6.0, 2},
    };
}

bool EnvioPedidosDemo::enviar(const Pedido& pedido) {
    pedidos_.push_back(pedido);
    return true; // Confirmação simulada; não encaminha solicitação a terceiros.
}

const std::vector<Pedido>& EnvioPedidosDemo::pedidosRegistrados() const noexcept {
    return pedidos_;
}

} // namespace autoestoque
