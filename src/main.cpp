#include <QApplication>

#include "demo/SimulacaoReposicao.hpp"
#include "gui/mainwindow.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // As interfaces externas são simuladas nesta versão da aplicação Qt.
    autoestoque::ConsultaFiliaisDemo consulta;
    autoestoque::EnvioPedidosDemo envio;

    MainWindow janela(consulta, envio);
    janela.show();
    return app.exec();
}
