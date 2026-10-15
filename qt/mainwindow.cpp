#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "bib.hpp"
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->botaoVender, &QPushButton::clicked,
            this, &MainWindow::registrarVenda);
    connect(ui->quantidade, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::atualizarStatus);
    connect(ui->pontoPedido, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::atualizarStatus);
    atualizarStatus();
}
MainWindow::~MainWindow() { delete ui; }
void MainWindow::registrarVenda() {
    if (ui->quantidade->value() > 0)
        ui->quantidade->setValue(ui->quantidade->value() - 1);
    else
        ui->alerta->setText("Venda recusada: estoque zerado");
}
void MainWindow::atualizarStatus() {
    const int quantidade = ui->quantidade->value();
    const int limite = ui->pontoPedido->value();
    const bool precisaRepor = estoqueAtingiuPontoPedido(quantidade, limite);
    ui->alerta->setText(precisaRepor
        ? "ALERTA: produto atingiu o ponto de pedido"
        : "Estoque acima do ponto de pedido");
    ui->botaoVender->setEnabled(quantidade > 0);
}
