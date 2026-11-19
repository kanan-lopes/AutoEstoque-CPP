# Integração da mini demo com o Serviço de Reposição — Qt Widgets

## Objetivo

Construir uma demonstração visual, utilizável por mouse e teclado, para as três operações tipadas da API v2: `consultarDisponibilidade`, `sugerirTransferencia` e `gerarPedidoAoFornecedor`. As regras de negócio continuam centralizadas no serviço C++17; a interface apenas coleta valores, chama o serviço e exibe resultados.

## Alterações em relação à branch `feat/sprint03-api-reposicao`

| Arquivo/diretório | Ação | Finalidade |
|---|---|---|
| `src/main.cpp` | Atualizado | Passa a inicializar QApplication e MainWindow; não existe um quarto `main.cpp` |
| `src/gui/mainwindow.ui` | Novo | Formulário Qt Designer com produto, F01, F02 e F03 |
| `src/gui/mainwindow.hpp/.cpp` | Novos | Conexões de sinais, slots e apresentação dos retornos da API |
| `src/demo/SimulacaoReposicao.hpp/.cpp` | Novos | Implementações demonstrativas de `IConsultaFiliais` e `IEnvioPedidos` |
| `qt/AutoEstoqueQt.pro` | Novo | Projeto qmake Qt Widgets/C++17 |
| `qt/tests/GuiTests.pro` | Novo | Projeto qmake da suíte Qt Test |
| `test/gui/test_gui.cpp` | Novo | 14 casos automatizados da GUI |
| `Makefile` | Atualizado | Compila as suítes C++ sem `src/main.cpp` (agora dependente de Qt) |
| `.gitignore` | Atualizado | Ignora binários e arquivos gerados por Qt Creator/qmake |
| `README.md` | Atualizado | Instruções de uso, testes e escopo |
| `src/ServicoReposicao.hpp/.cpp` | Preservados | Contratos/implementações da API v2 |
| `test/funcional/` e `test/unit/` | Preservados | 13 cenários e 9 verificações previamente existentes |

## Dependências entre camadas

- `src/gui` depende do contrato tipado da API em `src/ServicoReposicao.hpp`.
- `src/demo` implementa as portas `IConsultaFiliais` e `IEnvioPedidos` da API.
- `src/main.cpp` cria os adaptadores demo e os fornece ao construtor de MainWindow.
- MainWindow cria `ServicoReposicao` com os parâmetros informados no formulário e delega a ele o processamento.
- Os testes Qt criam seus próprios dublês de interface, de modo que os resultados não dependem do catálogo demonstrativo real.

## Como integrar no repositório

1. Entre na raiz do `AutoEstoque-CPP` no VS Code. Confira `git status` antes de alterar arquivos.
2. Partindo de `feat/sprint03-api-reposicao`, crie a branch `feat/qt-integracao-reposicao`.
3. Com o ZIP baixado em Downloads, execute no PowerShell:

```powershell
# Execute estando na raiz do repositório:
tar -xf "$HOME\Downloads\AutoEstoque-Qt-Integracao.zip" --strip-components=1 -C .
```

4. Verifique as alterações com `git status` e `git diff --stat`.
5. Execute `mingw32-make funcional` e `mingw32-make unit`.
6. No Qt Creator, abra `qt/AutoEstoqueQt.pro`, compile e execute; depois faça o mesmo com `qt/tests/GuiTests.pro`.
7. Depois de validar localmente, faça commit e push na branch de integração e abra PR para `develop` segundo o processo da disciplina.

**Não aplique o ZIP sobre `feat/mini-demo-qt`**, pois essa branch está na base antiga de `src` e `test`. Não faça `git merge` dessa branch antiga na branch nova; a nova interface já incorpora o comportamento do alerta de estoque da mini demo.

## Diferença entre os testes

- **Testes funcionais da API** (`test/funcional`): três funções, uma para cada funcionalidade, total de 13 cenários.
- **Testes unitários da API** (`test/unit`): nove verificações de regras e limites.
- **Testes de GUI** (`test/gui`): 14 verificações de interação/visualização com Qt Test. Reutilizam a API real, porém substituem as portas externas por dublês.

As suites C++ são independentes do Qt. O Qt Test somente pode ser compilado com um kit Qt apropriado; não considerar aprovado até executá-lo no ambiente local.

## Mapeamento GUI → API

| Ação na interface | Método invocado | Resultado visível |
|---|---|---|
| Consultar filiais elegíveis | F01 `consultarDisponibilidade(produto)` | Tabela de ofertas filtradas e ordenadas |
| Obter sugestão | F02 `sugerirTransferencia(produto)` | Identificador da filial e estoque disponível |
| Gerar pedido (simulado) | F03 `gerarPedidoAoFornecedor(produto, loja, recusa)` | Status e quantidade solicitada |
| Simular venda | `estoqueAtingiuPontoPedido()` após reduzir quantidade | Alerta do ponto de pedido (mini demo anterior) |

O resultado `AguardandoDecisao` não efetua compra; o usuário precisa marcar a recusa antes de gerar o pedido caso exista transferência viável.
