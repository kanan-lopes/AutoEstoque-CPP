#pragma once

#include "ServicoReposicao.hpp"

#include <QWidget>
#include <memory>

namespace Ui { class MainWindow; }

// A GUI converte ações do usuário em chamadas à API, sem implementar
// as regras de filtragem de filiais ou emissão de pedidos.
class MainWindow final : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(autoestoque::IConsultaFiliais& consulta,
                        autoestoque::IEnvioPedidos& envio,
                        QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void consultarFiliais();
    void sugerirTransferencia();
    void gerarPedido();
    void registrarVenda();
    void restaurarExemplo();
    void atualizarEstado();
    void limparResultados();

private:
    std::unique_ptr<Ui::MainWindow> ui_;
    autoestoque::IConsultaFiliais& consulta_;
    autoestoque::IEnvioPedidos& envio_;

    autoestoque::Produto obterProduto() const;
    autoestoque::ConfiguracaoReposicao obterConfiguracao() const;
    void configurarTabela();
};
