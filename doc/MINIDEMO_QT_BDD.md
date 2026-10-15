# Mini demo Qt — Roteiro e BDD

## Escopo
Tela Qt Widgets criada em Qt Designer (`qt/mainwindow.ui`). Demonstra um único produto fixo e o comportamento de atingir o ponto de pedido, reutilizando a lógica `estoqueAtingiuPontoPedido` de `src/bib.cpp`. Não há persistência, cadastro, consulta a filiais ou pedidos automáticos. A venda é simulada pelo botão.

## Cenários BDD

**Cenário 1: estoque acima do ponto de pedido**
- Given: produto com 6 unidades e ponto de pedido igual a 5.
- When: a GUI é aberta.
- Then: apresenta "Estoque acima do ponto de pedido".

**Cenário 2: venda atinge o ponto de pedido**
- Given: produto com 6 unidades e ponto de pedido 5.
- When: clico em "Registrar venda de 1 unidade".
- Then: quantidade passa a 5 e aparece o alerta de reposição.

**Cenário 3: produto sem unidades**
- Given: o estoque é alterado para 0.
- When: o estado da GUI é atualizado.
- Then: o botão de venda fica desabilitado.

## Como abrir no Qt Creator (Windows)
1. Abrir `qt/AutoEstoqueQt.pro` no Qt Creator e configurar um kit Desktop Qt.
2. Compilar e executar o projeto para visualizar a tela.
3. Abrir `qt/tests/GuiTests.pro` como projeto de testes separado, selecionar kit Qt Desktop e executar a suíte (ou acionar Run Tests).
4. Conferir as três verificações PASS no painel/output do Qt Test.

## Roteiro de vídeo (máximo 5 minutos)
- 0:00–0:30: apresentar estrutura do repositório e escopo limitado.
- 0:30–1:15: slides com os 3 cenários BDD.
- 1:15–2:15: abrir interface e fazer a venda simulada.
- 2:15–3:40: executar os testes de GUI no Qt Creator, mostrar seus resultados reais.
- 3:40–4:40: mostrar `QTest::mouseClick`, `findChild`, `QCOMPARE` e o slot `registrarVenda`.
- 4:40–5:00: explicar o que não foi implementado e o próximo incremento.

**Atenção:** este projeto foi gerado sem ambiente Qt instalado neste contêiner; compilação e execução dos testes Qt exigem validação no Qt Creator local. Não declarar aprovação dos testes antes da execução real.
