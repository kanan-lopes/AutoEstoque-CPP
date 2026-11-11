# AutoEstoque — Sprint 03 | API C++ de Reposição de Estoque

Implementação da camada de lógica de negócio com **um serviço** e **três funcionalidades**, conforme a especificação API v2:

- **F01** — Consultar disponibilidade de produto em filiais próximas.
- **F02** — Sugerir transferência entre lojas quando o produto atingir o ponto de pedido.
- **F03** — Gerar pedido ao fornecedor se necessário, respeitando a decisão sobre transferência.

As integrações externas com outras lojas e fornecedores são abstraídas por `IConsultaFiliais` e `IEnvioPedidos`. A demonstração e os testes utilizam implementações simuladas, sem comunicação real de rede ou envio real de e-mail.

## Organização

```text
AutoEstoque-CPP/
├── bin/                       # .o, .exe e demais binários (ignorados no Git)
├── src/
│   ├── main.cpp               # executável de demonstração do produto
│   ├── ServicoReposicao.hpp   # contrato da API v2
│   ├── ServicoReposicao.cpp   # implementação da API v2
│   ├── bib.hpp                # regras antigas de estoque (mantidas)
│   └── bib.cpp
├── test/
│   ├── funcional/
│   │   ├── main.cpp
│   │   ├── funcional_tests.h
│   │   └── funcional_tests.cpp
│   └── unit/
│       ├── main.cpp
│       ├── unit_tests.h
│       └── unit_tests.cpp
├── qt/                        # mini demo Qt existente em branch separada
├── doc/
├── Makefile
└── .gitignore
```

A branch da Sprint 03 não incorpora automaticamente os arquivos da branch `feat/mini-demo-qt`; os dois incrementos devem ser integrados posteriormente em `develop`.

## Build e execução (g++ e GNU Make)

```bash
make
./bin/autoestoque
make funcional
make unit
make test
make clean
```

No Windows com MinGW/MSYS2, os executáveis gerados são `bin/autoestoque.exe`, `bin/testeRegressivo.exe` e `bin/testeUnitario.exe`. Rode `make test` em um terminal com `make` e `g++` disponíveis.

## Rastreabilidade

| Funcionalidade | Teste funcional em `funcional_tests.cpp` | Casos cobertos |
|---|---|---|
| F01 | `consultarDisponibilidadeFuncionalTest()` | T01–T04 |
| F02 | `sugerirTransferenciaFuncionalTest()` | T01–T03 |
| F03 | `gerarPedidoAoFornecedorFuncionalTest()` | T01–T06 |

**O arquivo `test/funcional/main.cpp` chama exatamente essas três funções.** Elas reúnem 13 verificações de cenários especificados anteriormente; `test/unit/` contém nove verificações isoladas de validação, limites e parâmetros da mesma API.

Os três executáveis têm cada um seu próprio `main.cpp`, sem conflito na compilação.
