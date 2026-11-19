#pragma once

#include "ServicoReposicao.hpp"

#include <string>
#include <vector>

namespace autoestoque {

// Dublê demonstrativo: não acessa rede, banco de dados ou filiais reais.
class ConsultaFiliaisDemo final : public IConsultaFiliais {
public:
    std::vector<OfertaFilial> consultar(const std::string& sku) const override;
};

// Dublê demonstrativo: registra pedidos apenas em memória.
class EnvioPedidosDemo final : public IEnvioPedidos {
public:
    bool enviar(const Pedido& pedido) override;
    const std::vector<Pedido>& pedidosRegistrados() const noexcept;

private:
    std::vector<Pedido> pedidos_;
};

} // namespace autoestoque
