QT += core gui widgets testlib
CONFIG += c++17 testcase
TEMPLATE = app
TARGET = testeGuiAutoEstoque
DESTDIR = $$clean_path($$PWD/../../bin)

SOURCES += \
    ../../test/gui/test_gui.cpp \
    ../../src/gui/mainwindow.cpp \
    ../../src/ServicoReposicao.cpp \
    ../../src/bib.cpp

HEADERS += \
    ../../src/gui/mainwindow.hpp \
    ../../src/ServicoReposicao.hpp \
    ../../src/bib.hpp

FORMS += ../../src/gui/mainwindow.ui
INCLUDEPATH += ../../src ../../src/gui
