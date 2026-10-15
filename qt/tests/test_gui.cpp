#include <QtTest/QtTest>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include "mainwindow.hpp"

class GuiTests : public QObject {
    Q_OBJECT
private slots:
    void estadoInicialAcimaDoLimite();
    void vendaAtingePontoDePedido();
    void estoqueZeradoBloqueiaVenda();
};
void GuiTests::estadoInicialAcimaDoLimite() {
    MainWindow w;
    auto *alerta = w.findChild<QLabel*>("alerta");
    QVERIFY(alerta != nullptr);
    QCOMPARE(alerta->text(), QString("Estoque acima do ponto de pedido"));
}
void GuiTests::vendaAtingePontoDePedido() {
    MainWindow w;
    auto *qtd = w.findChild<QSpinBox*>("quantidade");
    auto *botao = w.findChild<QPushButton*>("botaoVender");
    auto *alerta = w.findChild<QLabel*>("alerta");
    QVERIFY(qtd && botao && alerta);
    QCOMPARE(qtd->value(), 6);
    QTest::mouseClick(botao, Qt::LeftButton);
    QCOMPARE(qtd->value(), 5);
    QCOMPARE(alerta->text(), QString("ALERTA: produto atingiu o ponto de pedido"));
}
void GuiTests::estoqueZeradoBloqueiaVenda() {
    MainWindow w;
    auto *qtd = w.findChild<QSpinBox*>("quantidade");
    auto *botao = w.findChild<QPushButton*>("botaoVender");
    QVERIFY(qtd && botao);
    qtd->setValue(0);
    QVERIFY(!botao->isEnabled());
    QCOMPARE(qtd->value(), 0);
}
QTEST_MAIN(GuiTests)
#include "test_gui.moc"
