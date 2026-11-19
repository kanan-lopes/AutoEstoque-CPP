# Cenários de testes da interface Qt — BDD

Os testes abaixo exercitam `test/gui/test_gui.cpp`, com `QTest::mouseClick()` e verificações dos componentes do formulário. **Esses testes são adicionais** aos 13 cenários funcionais e 9 verificações unitárias da API v2.

| ID | DADO QUE | QUANDO | ENTÃO |
|---|---|---|---|
| GUI-01 | Quantidade 5 e ponto 5 | Abro a janela | O alerta indica necessidade de reposição |
| GUI-02 | Quantidade 6 e ponto 5 | Clico em Simular venda | Quantidade passa para 5 e o alerta muda |
| GUI-03 | Quantidade atual 0 | Tento vender | O botão de venda está desabilitado |
| GUI-04 | FILIAL-A 4 km/8 un.; FILIAL-B 12 km; FILIAL-C 2 un. | Consulto filiais com 10 km e mínimo 3 | Somente FILIAL-A é mostrada na tabela |
| GUI-05 | Quantidade mínima configurada como 100 | Consulto filiais | A tabela fica vazia |
| GUI-06 | Estoque 5 e ponto 5; FILIAL-A elegível | Solicito sugestão | FILIAL-A e 8 unidades aparecem |
| GUI-07 | Estoque 6 e ponto 5 | Solicito sugestão | Nenhuma sugestão é apresentada |
| GUI-08 | Estoque 5, máximo 20, sem filial elegível | Solicito pedido | Pedido de 15 unidades é registrado em memória |
| GUI-09 | Existe transferência elegível; recusa não marcada | Solicito pedido | Sistema aguarda decisão e não envia pedido |
| GUI-10 | Existe transferência elegível; recusa marcada | Solicito pedido | Pedido é gerado na simulação |
| GUI-11 | Estoque 6 e ponto 5 | Solicito pedido | Estoque adequado; nada é enviado |
| GUI-12 | Fornecedor está vazio | Solicito pedido | Retorna dados inválidos; nada é enviado |
| GUI-13 | A porta de envio simulada falha | Solicito pedido sem filial elegível | GUI mostra falha no envio |
| GUI-14 | Resultado de consulta já está apresentado | Altero a quantidade do produto | A tabela é limpa para evitar dados obsoletos |

As portas externas são substituídas por dublês `ConsultaFake` e `EnvioFake`, definidos no próprio arquivo de teste. A API chamada pelos botões é a implementação real `ServicoReposicao`.

## Executar no Qt Creator

1. Abrir `qt/tests/GuiTests.pro`.
2. Selecionar kit Qt Desktop com módulo Qt Test.
3. Compilar e executar; observar a saída dos 14 métodos e do teste de inicialização/encerramento da suíte.
4. Confirmar `0 failed` e registrar a evidência de execução.
