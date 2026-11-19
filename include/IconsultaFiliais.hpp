#pragma once

#include <string>
#include <vector>
#include "TiposDominio.hpp"

namespace autoestoque {

// Porta de saída para consulta de estoque nas filiais da rede.
class IConsultaFiliais {
public:
    virtual ~IConsultaFiliais() = default;
    virtual std::vector<OfertaFilial> consultar(const std::string& sku) const = 0;
};

} // namespace autoestoque
