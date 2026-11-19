#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "bib.hpp"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <stdexcept>
#include <utility>

using autoestoque::ConfiguracaoReposicao;
using autoestoque::Produto;
using autoestoque::ServicoReposicao;
using autoestoque::StatusPedido;

MainWindow::MainWindow(autoestoque::IConsultaFiliais& consulta,
                       autoestoque::IEnvioPedidos& envio, QWidget* parent)
    : QWidget(parent),
      ui_(std::make_unique<Ui::MainWindow>()),
      consulta_(consulta),
      envio_(envio) {
    ui_->setupUi(this);
    configurarTabela();

    connect(ui_->botaoVender, &QPushButton::clicked,
            this, &MainWindow::registrarVenda);
    connect(ui_->botaoConsultarFiliais, &QPushButton::clicked,
            this, &MainWindow::consultarFiliais);
    connect(ui_->botaoSugerirTransferencia, &QPushButton::clicked,
            this, &MainWindow::sugerirTransferencia);
    connect(ui_->botaoGerarPedido, &QPushButton::clicked,
            this, &MainWindow::gerarPedido);
    connect(ui_->botaoRestaurar, &QPushButton::clicked,
            this, &MainWindow::restaurarExemplo);

    auto entradasAlteradas = [this]() {
        atualizarEstado();
        limparResultados();
    };
    for (QSpinBox* campo : {ui_->quantidade, ui_->pontoPedido,
                            ui_->estoqueMaximo, ui_->quantidadeMinima}) {
        connect(campo, QOverload<int>::of(&QSpinBox::valueChanged),
                this, entradasAlteradas);
    }
    connect(ui_->raioMaximo,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, entradasAlteradas);
    for (QLineEdit* campo : {ui_->sku, ui_->fornecedorId,
                             ui_->lojaSolicitante}) {
        connect(campo, &QLineEdit::textChanged, this, entradasAlteradas);
    }
    connect(ui_->checkRecusarTransferencia, &QCheckBox::toggled,
            this, [this](bool) {
                ui_->labelPedido->setText("Nenhum pedido solicitado nesta configuração.");
            });

    setStyleSheet(
        "QGroupBox { font-weight: 600; margin-top: 9px; padding-top: 9px; }"
        "QPushButton { padding: 6px 12px; }"
        "QTableWidget { alternate-background-color: #f4f7fb; }"
        "QLabel#titulo { font-size: 19px; font-weight: 700; }"
    );
    atualizarEstado();
    limparResultados();
}

MainWindow::~MainWindow() = default;

Produto MainWindow::obterProduto() const {
    return {
        ui_->sku->text().trimmed().toStdString(),
        ui_->fornecedorId->text().trimmed().toStdString(),
        ui_->quantidade->value(),
        ui_->pontoPedido->value(),
        ui_->estoqueMaximo->value()
    };
}

ConfiguracaoReposicao MainWindow::obterConfiguracao() const {
    return {ui_->raioMaximo->value(), ui_->quantidadeMinima->value()};
}

void MainWindow::configurarTabela() {
    ui_->tabelaFiliais->setColumnCount(3);
    ui_->tabelaFiliais->setHorizontalHeaderLabels({"Filial", "Distância (km)", "Unidades"});
    ui_->tabelaFiliais->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui_->tabelaFiliais->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_->tabelaFiliais->setSelectionMode(QAbstractItemView::NoSelection);
    ui_->tabelaFiliais->setAlternatingRowColors(true);
}

void MainWindow::atualizarEstado() {
    const int atual = ui_->quantidade->value();
    const int ponto = ui_->pontoPedido->value();
    const bool precisaRepor = estoqueAtingiuPontoPedido(atual, ponto);
    ui_->statusEstoque->setText(precisaRepor
        ? "ATENÇÃO: estoque no ponto de pedido (ou abaixo). Reposição indicada."
        : "Estoque acima do ponto de pedido.");
    ui_->botaoVender->setEnabled(atual > 0);
}

void MainWindow::limparResultados() {
    ui_->tabelaFiliais->setRowCount(0);
    ui_->labelConsulta->setText("Consulta ainda não executada.");
    ui_->labelSugestao->setText("Sugestão ainda não solicitada.");
    ui_->labelPedido->setText("Pedido ainda não solicitado.");
}

void MainWindow::registrarVenda() {
    if (ui_->quantidade->value() > 0) {
        // A mudança dispara o sinal valueChanged e atualiza o alerta.
        ui_->quantidade->setValue(ui_->quantidade->value() - 1);
    }
}

void MainWindow::restaurarExemplo() {
    ui_->sku->setText("SSD480");
    ui_->fornecedorId->setText("FORN01");
    ui_->lojaSolicitante->setText("MATRIZ");
    ui_->quantidade->setValue(5);
    ui_->pontoPedido->setValue(5);
    ui_->estoqueMaximo->setValue(20);
    ui_->raioMaximo->setValue(10.0);
    ui_->quantidadeMinima->setValue(3);
    ui_->checkRecusarTransferencia->setChecked(false);
    atualizarEstado();
    limparResultados();
}

void MainWindow::consultarFiliais() {
    try {
        ServicoReposicao servico(consulta_, envio_, obterConfiguracao());
        const auto ofertas = servico.consultarDisponibilidade(obterProduto());
        ui_->tabelaFiliais->setRowCount(static_cast<int>(ofertas.size()));
        for (int i = 0; i < static_cast<int>(ofertas.size()); ++i) {
            const auto& oferta = ofertas[static_cast<std::size_t>(i)];
            ui_->tabelaFiliais->setItem(i, 0,
                new QTableWidgetItem(QString::fromStdString(oferta.lojaId)));
            ui_->tabelaFiliais->setItem(i, 1,
                new QTableWidgetItem(QString::number(oferta.distanciaKm, 'f', 1)));
            ui_->tabelaFiliais->setItem(i, 2,
                new QTableWidgetItem(QString::number(oferta.quantidade)));
        }
        ui_->labelConsulta->setText(ofertas.empty()
            ? "Nenhuma filial elegível encontrada (ou dados do produto inválidos)."
            : QString("Filiais elegíveis: %1 (ordenadas por distância).")
                .arg(ofertas.size()));
    } catch (const std::invalid_argument&) {
        ui_->tabelaFiliais->setRowCount(0);
        ui_->labelConsulta->setText("Parâmetros de reposição inválidos.");
    }
}

void MainWindow::sugerirTransferencia() {
    try {
        ServicoReposicao servico(consulta_, envio_, obterConfiguracao());
        const auto sugestao = servico.sugerirTransferencia(obterProduto());
        if (sugestao.disponivel) {
            ui_->labelSugestao->setText(
                QString("Transferência sugerida: %1; estoque na filial: %2 unidades. "
                        "Nenhuma transferência foi executada.")
                    .arg(QString::fromStdString(sugestao.lojaId))
                    .arg(sugestao.quantidade));
        } else {
            ui_->labelSugestao->setText(
                "Nenhuma sugestão disponível para os dados e limites informados.");
        }
    } catch (const std::invalid_argument&) {
        ui_->labelSugestao->setText("Parâmetros de reposição inválidos.");
    }
}

void MainWindow::gerarPedido() {
    try {
        ServicoReposicao servico(consulta_, envio_, obterConfiguracao());
        const auto resultado = servico.gerarPedidoAoFornecedor(
            obterProduto(), ui_->lojaSolicitante->text().trimmed().toStdString(),
            ui_->checkRecusarTransferencia->isChecked());

        switch (resultado.status) {
        case StatusPedido::Gerado:
            ui_->labelPedido->setText(
                QString("Pedido gerado (envio simulado): %1 unidades para o fornecedor %2.")
                    .arg(resultado.quantidadeSolicitada)
                    .arg(ui_->fornecedorId->text().trimmed()));
            break;
        case StatusPedido::AguardandoDecisao:
            ui_->labelPedido->setText(
                "Aguardando decisão: existe filial elegível. "
                "Marque 'Transferência recusada' para autorizar compra externa.");
            break;
        case StatusPedido::EstoqueAdequado:
            ui_->labelPedido->setText("Estoque adequado: nenhum pedido foi gerado.");
            break;
        case StatusPedido::DadosInvalidos:
            ui_->labelPedido->setText("Dados inválidos: pedido não gerado.");
            break;
        case StatusPedido::FalhaEnvio:
            ui_->labelPedido->setText("Falha no envio: pedido não confirmado.");
            break;
        }
    } catch (const std::invalid_argument&) {
        ui_->labelPedido->setText("Parâmetros de reposição inválidos.");
    }
}
