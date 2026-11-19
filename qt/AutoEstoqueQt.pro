QT += core gui widgets
CONFIG += c++17
TEMPLATE = app
TARGET = AutoEstoqueQt

# O binário gerado fica em bin; objetos ficam no diretório de build do Qt Creator.
DESTDIR = $$clean_path($$PWD/../bin)

SOURCES += \
    ../src/main.cpp \
    ../src/gui/mainwindow.cpp \
    ../src/demo/SimulacaoReposicao.cpp \
    ../src/ServicoReposicao.cpp \
    ../src/bib.cpp

HEADERS += \
    ../src/gui/mainwindow.hpp \
    ../src/demo/SimulacaoReposicao.hpp \
    ../src/ServicoReposicao.hpp \
    ../src/bib.hpp

FORMS += ../src/gui/mainwindow.ui
INCLUDEPATH += ../src ../src/gui ../src/demo
