#ifndef MENUFUNCIONARIOWINDOW_H
#define MENUFUNCIONARIOWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QStackedWidget>
#include <QTime>
#include <vector>
#include "Usuario.hpp"
#include "FilaPrioridade.hpp"
#include "Database.hpp"
#include "Triagem.hpp"

class Paciente;

class MenuFuncionarioWindow : public QWidget {
    Q_OBJECT
public:
    MenuFuncionarioWindow(Usuario* usuario, std::vector<Usuario*>& usuarios,
                          FilaPrioridade& fila, Database* db, QWidget* parent = nullptr);
private slots:
    void onChamarProximo();
    void onEditarDados();
    void onSolicSelecionar(int row);
    void onSolicAceitar();
    void onSolicNegar();
    void onTriagemBuscar();
    void onTriagemConfirmar();
    void onVoltar();
    void atualizarDashboard();
    void atualizarSolicitacoes();
    void atualizarFila();
    void atualizarHistorico();

private:

    int atendimentosHoje = 0;
    int atendimentosRealizados = 0;

    Usuario*               usuario;
    std::vector<Usuario*>& usuarios;
    FilaPrioridade&        fila;
    Database*              db;

    QStackedWidget* stackContent = nullptr;
    QPushButton* btnDash   = nullptr;
    QPushButton* btnSolics = nullptr;
    QPushButton* btnTriag  = nullptr;
    QPushButton* btnFila   = nullptr;
    QPushButton* btnCad    = nullptr;
    QPushButton* btnHist   = nullptr;
    QPushButton* btnEdit   = nullptr;

    // Dashboard
    QLabel* lblStatSolic = nullptr;
    QLabel* lblStatAtend = nullptr;
    QLabel* lblStatFila  = nullptr;
    QLabel* lblStatTotal = nullptr;
    QLabel* lblDashAlta  = nullptr;
    QLabel* lblDashMedia = nullptr;
    QLabel* lblDashBaixa = nullptr;
    QLabel* lblDashMuito = nullptr;
    QLabel* lblProxNome  = nullptr;
    QLabel* lblProxPri   = nullptr;

    // Solicitações
    QListWidget* lstSolic        = nullptr;
    QTextEdit*   txtSolicDetalhe = nullptr;
    QLineEdit*   edtTipoSolic    = nullptr;
    QComboBox*   cboPriSolic     = nullptr;
    std::vector<Paciente*> pacientesComSol;

    // Triagem
    QLineEdit*   edtTriagemLogin  = nullptr;
    QLabel*      lblTriagemNome   = nullptr;
    QLabel*      lblTriagemCurso  = nullptr;
    QLabel*      lblTriagemIdade  = nullptr;
    QTextEdit*   txtTriagemSint   = nullptr;
    QComboBox*   cboTriagemTipo   = nullptr;
    QPushButton* btnPri1 = nullptr;
    QPushButton* btnPri2 = nullptr;
    QPushButton* btnPri3 = nullptr;
    QPushButton* btnPri4 = nullptr;
    QLabel*      lblTriagemErro   = nullptr;
    Paciente*    pacienteTriagem  = nullptr;

    // Fila
    QListWidget* lstFilaAlta  = nullptr;
    QListWidget* lstFilaMedia = nullptr;
    QListWidget* lstFilaBaixa = nullptr;
    QListWidget* lstFilaMuito = nullptr;

    // Histórico
    QTextEdit* txtHistorico = nullptr;

    void buildPageDashboard(QWidget* pg);
    void buildPageSolicitacoes(QWidget* pg);
    void buildPageTriagem(QWidget* pg);
    void buildPageFila(QWidget* pg);
    void buildPageCadastro(QWidget* pg);
    void buildPageHistorico(QWidget* pg);
    void buildPageEditar(QWidget* pg);

    QWidget* makeCard(QWidget* parent, const QString& titulo = "");
    QLabel*  makeStat(QWidget* parent, const QString& num, const QString& desc, const QString& bg);
    void applyStyle();
};
#endif
