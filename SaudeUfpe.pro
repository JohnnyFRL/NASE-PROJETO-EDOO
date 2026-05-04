QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = NaseUfpe
TEMPLATE = app

SOURCES += \
    main_qt.cpp \
    stubs.cpp \
    Pessoa.cpp \
    Usuario.cpp \
    Paciente.cpp \
    Funcionario.cpp \
    Triagem.cpp \
    FilaPrioridade.cpp \
    Validacao.cpp \
    MainWindow.cpp \
    LoginDialog.cpp \
    CadastroDialog.cpp \
    MenuPacienteWindow.cpp \
    MenuFuncionarioWindow.cpp \
    TriagemDialog.cpp \
    SolicitacoesDialog.cpp

HEADERS += \
    Pessoa.hpp \
    Usuario.hpp \
    Paciente.hpp \
    Funcionario.hpp \
    Triagem.hpp \
    FilaPrioridade.hpp \
    Validacao.hpp \
    MainWindow.h \
    LoginDialog.h \
    CadastroDialog.h \
    MenuPacienteWindow.h \
    MenuFuncionarioWindow.h \
    NaseStyle.h \
    TriagemDialog.h \
    SolicitacoesDialog.h
-e 
RESOURCES += resources.qrc

SOURCES += EditarDadosDialog.cpp
HEADERS += EditarDadosDialog.h
