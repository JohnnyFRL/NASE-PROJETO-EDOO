QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = SaudeUFPE
TEMPLATE = app

INCLUDEPATH += include

SOURCES += \
    src/main_qt.cpp \
    src/stubs.cpp \
    src/Database.cpp \
    src/Pessoa.cpp \
    src/Usuario.cpp \
    src/Paciente.cpp \
    src/Funcionario.cpp \
    src/Triagem.cpp \
    src/FilaPrioridade.cpp \
    src/Validacao.cpp \
    src/MainWindow.cpp \
    src/LoginDialog.cpp \
    src/CadastroDialog.cpp \
    src/MenuPacienteWindow.cpp \
    src/MenuFuncionarioWindow.cpp \
    src/TriagemDialog.cpp \
    src/SolicitacoesDialog.cpp \
    src/EditarDadosDialog.cpp

HEADERS += \
    include/Database.hpp \
    include/Pessoa.hpp \
    include/Usuario.hpp \
    include/Paciente.hpp \
    include/Funcionario.hpp \
    include/Triagem.hpp \
    include/FilaPrioridade.hpp \
    include/Validacao.hpp \
    include/NaseStyle.h \
    include/MainWindow.h \
    include/LoginDialog.h \
    include/CadastroDialog.h \
    include/MenuPacienteWindow.h \
    include/MenuFuncionarioWindow.h \
    include/TriagemDialog.h \
    include/SolicitacoesDialog.h \
    include/EditarDadosDialog.h

RESOURCES += \
    resources/resources.qrc