#include <QtTest/QtTest>

#include "gui/mainwindow.hpp"

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>

#include <string>
#include <vector>

using namespace autoestoque;

namespace {

struct ConsultaFake final : IConsultaFiliais {
    std::vector<OfertaFilial> ofertas{
        {"FILIAL-B", 12.0, 9},
        {"FILIAL-A", 4.0, 8},
        {"FILIAL-C", 6.0, 2}
    };

    std::vector<OfertaFilial> consultar(const std::string&) const override {
        return ofertas;
    }
};

struct EnvioFake final : IEnvioPedidos {
    bool deveEnviar = true;
    std::vector<Pedido> recebidos;

    bool enviar(const Pedido& pedido) override {
        if (deveEnviar) {
            recebidos.push_back(pedido);
        }
        return deveEnviar;
    }
};

template <class T>
T* obter(MainWindow& janela, const char* id) {
    return janela.findChild<T*>(id);
}

void clicar(MainWindow& janela, const char* id) {
    auto* botao = obter<QPushButton>(janela, id);
    QVERIFY2(botao != nullptr, id);
    QTest::mouseClick(botao, Qt::LeftButton);
}

} // namespace

class GuiTests : public QObject {
    Q_OBJECT
private slots:
    void estadoInicialNoPontoDePedido();
    void vendaAtualizaAlerta();
    void semEstoqueBloqueiaVenda();
    void consultaMostraSomenteFilialElegivel();
    void consultaSemOfertaElegivel();
    void sugestaoDisponivelNoPonto();
    void sugestaoInexistenteAcimaDoPonto();
    void pedidoSemFiliaisElegiveis();
    void pedidoAguardaDecisaoDeTransferencia();
    void pedidoAposRecusaExplicita();
    void pedidoNaoGeradoComEstoqueAdequado();
    void pedidoNaoGeradoComFornecedorInvalido();
    void pedidoInformaFalhaDeEnvio();
    void alteracaoDeDadosLimpaResultadosAnteriores();
};

void GuiTests::estadoInicialNoPontoDePedido() {
    ConsultaFake consulta;
    EnvioFake envio;
    MainWindow janela(consulta, envio);
    auto* status = obter<QLabel>(janela, "statusEstoque");
    QVERIFY(status != nullptr);
    QVERIFY(status->text().contains("ponto de pedido"));
}

void GuiTests::vendaAtualizaAlerta() {
    ConsultaFake consulta;
    EnvioFake envio;
    MainWindow janela(consulta, envio);
    janela.show();
    auto* quantidade = obter<QSpinBox>(janela, "quantidade");
    auto* status = obter<QLabel>(janela, "statusEstoque");
    QVERIFY(quantidade && status);
    quantidade->setValue(6);
    QVERIFY(status->text().contains("acima"));
    clicar(janela, "botaoVender");
    QCOMPARE(quantidade->value(), 5);
    QVERIFY(status->text().contains("Reposição indicada"));
}

void GuiTests::semEstoqueBloqueiaVenda() {
    ConsultaFake consulta;
    EnvioFake envio;
    MainWindow janela(consulta, envio);
    auto* quantidade = obter<QSpinBox>(janela, "quantidade");
    auto* botao = obter<QPushButton>(janela, "botaoVender");
    QVERIFY(quantidade && botao);
    quantidade->setValue(0);
    QVERIFY(!botao->isEnabled());
}

void GuiTests::consultaMostraSomenteFilialElegivel() {
    ConsultaFake consulta;
    EnvioFake envio;
    MainWindow janela(consulta, envio);
    janela.show();
    clicar(janela, "botaoConsultarFiliais");
    auto* tabela = obter<QTableWidget>(janela, "tabelaFiliais");
    QVERIFY(tabela);
    QCOMPARE(tabela->rowCount(), 1);
    QCOMPARE(tabela->item(0, 0)->text(), QString("FILIAL-A"));
    QCOMPARE(tabela->item(0, 2)->text(), QString("8"));
}

void GuiTests::consultaSemOfertaElegivel() {
    ConsultaFake consulta;
    EnvioFake envio;
    MainWindow janela(consulta, envio);
    janela.show();
    auto* minimo = obter<QSpinBox>(janela, "quantidadeMinima");
    QVERIFY(minimo);
    minimo->setValue(100);
    clicar(janela, "botaoConsultarFiliais");
    auto* tabela = obter<QTableWidget>(janela, "tabelaFiliais");
    QVERIFY(tabela);
    QCOMPARE(tabela->rowCount(), 0);
}

void GuiTests::sugestaoDisponivelNoPonto() {
    ConsultaFake consulta;
    EnvioFake envio;
    MainWindow janela(consulta, envio);
    janela.show();
    clicar(janela, "botaoSugerirTransferencia");
    auto* label = obter<QLabel>(janela, "labelSugestao");
    QVERIFY(label);
    QVERIFY(label->text().contains("FILIAL-A"));
    QVERIFY(label->text().contains("8 unidades"));
}

void GuiTests::sugestaoInexistenteAcimaDoPonto() {
    ConsultaFake consulta;
    EnvioFake envio;
    MainWindow janela(consulta, envio);
    janela.show();
    auto* qtd = obter<QSpinBox>(janela, "quantidade");
    QVERIFY(qtd);
    qtd->setValue(6);
    clicar(janela, "botaoSugerirTransferencia");
    auto* label = obter<QLabel>(janela, "labelSugestao");
    QVERIFY(label);
    QVERIFY(label->text().contains("Nenhuma sugestão"));
}

void GuiTests::pedidoSemFiliaisElegiveis() {
    ConsultaFake consulta;
    consulta.ofertas.clear();
    EnvioFake envio;
    MainWindow janela(consulta, envio);
    janela.show();
    clicar(janela, "botaoGerarPedido");
    auto* label = obter<QLabel>(janela, "labelPedido");
    QVERIFY(label);
    QVERIFY(label->text().contains("15 unidades"));
    QCOMPARE(static_cast<int>(envio.recebidos.size()), 1);
    QCOMPARE(envio.recebidos[0].quantidade, 15);
}

void GuiTests::pedidoAguardaDecisaoDeTransferencia() {
    ConsultaFake consulta;
    EnvioFake envio;
    MainWindow janela(consulta, envio);
    janela.show();
    clicar(janela, "botaoGerarPedido");
    auto* label = obter<QLabel>(janela, "labelPedido");
    QVERIFY(label);
    QVERIFY(label->text().contains("Aguardando decisão"));
    QVERIFY(envio.recebidos.empty());
}

void GuiTests::pedidoAposRecusaExplicita() {
    ConsultaFake consulta;
    EnvioFake envio;
    MainWindow janela(consulta, envio);
    janela.show();
    auto* recusa = obter<QCheckBox>(janela, "checkRecusarTransferencia");
    QVERIFY(recusa);
    recusa->setChecked(true);
    clicar(janela, "botaoGerarPedido");
    auto* label = obter<QLabel>(janela, "labelPedido");
    QVERIFY(label);
    QVERIFY(label->text().contains("Pedido gerado"));
    QCOMPARE(static_cast<int>(envio.recebidos.size()), 1);
}

void GuiTests::pedidoNaoGeradoComEstoqueAdequado() {
    ConsultaFake consulta;
    EnvioFake envio;
    MainWindow janela(consulta, envio);
    janela.show();
    auto* qtd = obter<QSpinBox>(janela, "quantidade");
    QVERIFY(qtd);
    qtd->setValue(6);
    clicar(janela, "botaoGerarPedido");
    auto* label = obter<QLabel>(janela, "labelPedido");
    QVERIFY(label);
    QVERIFY(label->text().contains("Estoque adequado"));
    QVERIFY(envio.recebidos.empty());
}

void GuiTests::pedidoNaoGeradoComFornecedorInvalido() {
    ConsultaFake consulta;
    EnvioFake envio;
    MainWindow janela(consulta, envio);
    janela.show();
    auto* fornecedor = obter<QLineEdit>(janela, "fornecedorId");
    QVERIFY(fornecedor);
    fornecedor->clear();
    clicar(janela, "botaoGerarPedido");
    auto* label = obter<QLabel>(janela, "labelPedido");
    QVERIFY(label);
    QVERIFY(label->text().contains("Dados inválidos"));
    QVERIFY(envio.recebidos.empty());
}

void GuiTests::pedidoInformaFalhaDeEnvio() {
    ConsultaFake consulta;
    consulta.ofertas.clear();
    EnvioFake envio;
    envio.deveEnviar = false;
    MainWindow janela(consulta, envio);
    janela.show();
    clicar(janela, "botaoGerarPedido");
    auto* label = obter<QLabel>(janela, "labelPedido");
    QVERIFY(label);
    QVERIFY(label->text().contains("Falha no envio"));
    QVERIFY(envio.recebidos.empty());
}

void GuiTests::alteracaoDeDadosLimpaResultadosAnteriores() {
    ConsultaFake consulta;
    EnvioFake envio;
    MainWindow janela(consulta, envio);
    janela.show();
    clicar(janela, "botaoConsultarFiliais");
    auto* tabela = obter<QTableWidget>(janela, "tabelaFiliais");
    auto* quantidade = obter<QSpinBox>(janela, "quantidade");
    QVERIFY(tabela && quantidade);
    QCOMPARE(tabela->rowCount(), 1);
    quantidade->setValue(4);
    QCOMPARE(tabela->rowCount(), 0);
}

QTEST_MAIN(GuiTests)
#include "test_gui.moc"
