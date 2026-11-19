#pragma once

#include "TiposDominio.hpp"

namespace autoestoque {

// Porta de saída para envio de pedidos de compra ao fornecedor padrão.
class IEnvioPedidos {
public:
    virtual ~IEnvioPedidos() = default;
    virtual bool enviar(const Pedido& pedido) = 0;
};

} // namespace autoestoque
