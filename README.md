# AutoEstoque — Integração Qt com API v2 de Reposição

Versão demonstrativa do AutoEstoque com interface Qt Widgets, baseada no **Serviço de Reposição de Estoque** (API v2). A lógica de negócio permanece em C++17; a GUI é uma camada de apresentação.

## Funcionalidades disponíveis

- **Apoio ao monitoramento (mini demo anterior):** simular uma venda e atualizar o alerta quando a quantidade atinge o ponto de pedido.
- **F01 — Consultar disponibilidade:** identificar filiais dentro do raio máximo e com estoque mínimo configurado. Ofertas são ordenadas por distância.
- **F02 — Sugerir transferência:** informar a filial elegível mais próxima quando o estoque chega ao ponto de pedido.
- **F03 — Gerar pedido ao fornecedor:** confirmar solicitação de compra se não houver filial elegível ou se a transferência for explicitamente recusada. Se existir oferta e não houver recusa, a API retorna `AguardandoDecisao`.

Todas as integrações externas são **simuladas**: `ConsultaFiliaisDemo` fornece dados locais e `EnvioPedidosDemo` armazena pedidos em memória. Nenhum e-mail, compra, transferência física ou requisição de rede é realizada.

## Diretórios e entradas

```text
AutoEstoque-CPP/
├── bin/                         # binários locais ignorados pelo Git
├── src/
│   ├── main.cpp                 # main do produto Qt (substitui a antiga demo CLI)
│   ├── ServicoReposicao.hpp     # API v2 — mantida
│   ├── ServicoReposicao.cpp     # API v2 — mantida
│   ├── bib.hpp / bib.cpp        # regra do ponto de pedido — mantida
│   ├── gui/
│   │   ├── mainwindow.hpp
│   │   ├── mainwindow.cpp
│   │   └── mainwindow.ui         # formulário editável no Qt Designer
│   └── demo/
│       ├── SimulacaoReposicao.hpp
│       └── SimulacaoReposicao.cpp
├── test/
│   ├── funcional/               # três funções cobrindo 13 cenários
│   │   ├── main.cpp
│   │   ├── funcional_tests.h
│   │   └── funcional_tests.cpp
│   ├── unit/                    # 9 verificações unitárias
│   │   ├── main.cpp
│   │   ├── unit_tests.h
│   │   └── unit_tests.cpp
│   └── gui/
│       └── test_gui.cpp         # 14 testes Qt Test com QTEST_MAIN
├── qt/
│   ├── AutoEstoqueQt.pro        # projeto GUI
│   └── tests/GuiTests.pro       # projeto dos testes GUI
├── doc/
│   ├── INTEGRACAO_QT.md
│   ├── CENARIOS_GUI_QT.md
│   └── GUIA_GITFLOW.md
├── Makefile                     # testes C++ independentes do Qt
└── .gitignore
```

**Três arquivos `main.cpp` explícitos:** `src/main.cpp`, `test/funcional/main.cpp` e `test/unit/main.cpp`. O Qt Test declara sua entrada dentro de `test/gui/test_gui.cpp` pela macro `QTEST_MAIN`, sem criar outro arquivo chamado `main.cpp`.

## Compilação no Windows

### Aplicação Qt (Qt Creator)

1. Abra `qt/AutoEstoqueQt.pro` no **Qt Creator**.
2. Selecione um kit **Desktop Qt 6** instalado e configurado.
3. Compile e execute. O projeto compila os arquivos de `src` e gera `bin/AutoEstoqueQt.exe` (ou saída equivalente conforme o kit).
4. Em `src/gui/mainwindow.ui`, use **Open with Qt Designer** para editar a tela visualmente.

Use o mesmo kit de Qt para a aplicação e para os testes de GUI. Um Qt **MSVC** deve ser compilado com MSVC; um Qt **MinGW** deve ser compilado com o MinGW correspondente. O `mingw32-make` das suítes C++ abaixo não precisa linkar bibliotecas Qt.

### Testes Qt da interface

1. Abra `qt/tests/GuiTests.pro` no Qt Creator, como projeto separado.
2. Selecione o mesmo kit Desktop Qt utilizado na aplicação.
3. Compile e execute `testeGuiAutoEstoque` pelo Qt Creator. O projeto usa `Qt Test` (`QTEST_MAIN`, `QTest::mouseClick`, `QCOMPARE`).
4. Confirme a saída com os casos aprovados e **0 failed** antes de publicar evidência de aprovação.

### Testes da API C++ (terminal do VS Code)

```powershell
mingw32-make funcional
mingw32-make unit
# ou todos:
mingw32-make test
```

`mingw32-make` sem argumentos executa as duas suítes C++ e informa que o produto gráfico deve ser construído pelo Qt Creator. O executável do produto **não** é mais compilado pelo Makefile C++ puro porque `src/main.cpp` usa Qt.

## Uso manual da demo

**Configuração inicial:** SKU SSD480, fornecedor FORN01, loja MATRIZ, estoque atual 5, ponto de pedido 5, estoque máximo 20, raio 10 km, mínimo 3 unidades.

1. **F01:** clique em **Consultar filiais elegíveis**. Aparece FILIAL-A a 4 km com 8 unidades. FILIAL-B (12 km) e FILIAL-C (2 unidades) são filtradas.
2. **F02:** clique em **Obter sugestão**. A GUI mostra FILIAL-A. Não é realizada transferência física.
3. **F03, alternativa pendente:** clique em **Gerar pedido (simulado)** sem marcar recusa. Resultado: `AguardandoDecisao`, sem envio.
4. **F03, recusa:** marque **Transferência sugerida foi recusada** e clique novamente. Resultado: `Gerado`, quantidade 15 (20 − 5), envio simulado.
5. **F03, sem filial elegível:** restaure o exemplo, aumente **Quantidade mínima** para 100 e clique em gerar pedido. Não haverá oferta elegível; o pedido pode ser gerado sem recusa.
6. **Estoque adequado:** restaure o exemplo, altere estoque atual para 6 e teste F02 e F03: nenhuma sugestão e nenhum pedido.
7. **Venda simulada:** altere estoque atual para 6 e clique em **Simular venda (-1)**. A quantidade cai para 5 e o alerta muda.

Modificações nos parâmetros limpam resultados anteriores da interface para evitar mostrar informação desatualizada.

## Limites

- Não existem banco de dados, autenticação, serviços de rede, compra externa ou transferência real nesta versão.
- A quantidade informada na sugestão F02 é **estoque disponível na filial**, não quantidade efetivamente transferida.
- Os KPIs da API são especificados no documento de projeto, mas a interface ainda não registra séries históricas para calculá-los.
- A API v2 e os 13 cenários da Sprint 03 foram preservados, sem alterar as regras de reposição.

Veja `doc/INTEGRACAO_QT.md` para detalhes das mudanças e `doc/CENARIOS_GUI_QT.md` para rastreabilidade dos testes.
