/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titulo;
    QLabel *produto;
    QFormLayout *formLayout;
    QLabel *rotuloQuantidade;
    QSpinBox *quantidade;
    QLabel *rotuloPonto;
    QSpinBox *pontoPedido;
    QPushButton *botaoVender;
    QLabel *alerta;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->setMinimumSize(QSize(450, 240));
        verticalLayout = new QVBoxLayout(MainWindow);
        verticalLayout->setObjectName("verticalLayout");
        titulo = new QLabel(MainWindow);
        titulo->setObjectName("titulo");

        verticalLayout->addWidget(titulo);

        produto = new QLabel(MainWindow);
        produto->setObjectName("produto");

        verticalLayout->addWidget(produto);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        rotuloQuantidade = new QLabel(MainWindow);
        rotuloQuantidade->setObjectName("rotuloQuantidade");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, rotuloQuantidade);

        quantidade = new QSpinBox(MainWindow);
        quantidade->setObjectName("quantidade");
        quantidade->setMaximum(100000);
        quantidade->setValue(6);

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, quantidade);

        rotuloPonto = new QLabel(MainWindow);
        rotuloPonto->setObjectName("rotuloPonto");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, rotuloPonto);

        pontoPedido = new QSpinBox(MainWindow);
        pontoPedido->setObjectName("pontoPedido");
        pontoPedido->setMaximum(100000);
        pontoPedido->setValue(5);

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, pontoPedido);


        verticalLayout->addLayout(formLayout);

        botaoVender = new QPushButton(MainWindow);
        botaoVender->setObjectName("botaoVender");

        verticalLayout->addWidget(botaoVender);

        alerta = new QLabel(MainWindow);
        alerta->setObjectName("alerta");
        alerta->setWordWrap(true);

        verticalLayout->addWidget(alerta);


        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "AutoEstoque - Mini Demo", nullptr));
        titulo->setText(QCoreApplication::translate("MainWindow", "AutoEstoque | Monitoramento de Estoque", nullptr));
        produto->setText(QCoreApplication::translate("MainWindow", "Produto: SSD 480 GB (demonstrativo)", nullptr));
        rotuloQuantidade->setText(QCoreApplication::translate("MainWindow", "Quantidade atual", nullptr));
        rotuloPonto->setText(QCoreApplication::translate("MainWindow", "Ponto de pedido", nullptr));
        botaoVender->setText(QCoreApplication::translate("MainWindow", "Registrar venda de 1 unidade", nullptr));
        alerta->setText(QCoreApplication::translate("MainWindow", "Estado do estoque", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
