#include "NaseStyle.h"
#include "SolicitacoesDialog.h"
#include "Paciente.hpp"
#include "Triagem.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>

SolicitacoesDialog::SolicitacoesDialog(std::vector<Usuario*>& usuarios, FilaPrioridade& fila, QWidget* parent)
    : QDialog(parent), usuarios(usuarios), fila(fila) {
    setWindowTitle("Solicitações Pendentes");
    setModal(true);
    setMinimumSize(480, 500);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(10);

    auto* lblTit = new QLabel("Solicitações de Consulta Pendentes");
    lblTit->setObjectName("lblTitDlg");
    lblTit->setAlignment(Qt::AlignCenter);

    lstPacientes = new QListWidget();
    lstPacientes->setMaximumHeight(160);

    txtDetalhes = new QTextEdit();
    txtDetalhes->setReadOnly(true);
    txtDetalhes->setMaximumHeight(100);
    txtDetalhes->setPlaceholderText("Selecione um paciente para ver os detalhes...");

    auto* form = new QFormLayout();
    edtTipo = new QLineEdit();
    edtTipo->setPlaceholderText("ex: Consulta Médica");
    edtTipo->setMinimumHeight(34);
    cboPrioridade = new QComboBox();
    cboPrioridade->addItem("1 – Alta");
    cboPrioridade->addItem("2 – Média");
    cboPrioridade->addItem("3 – Baixa");
    cboPrioridade->setMinimumHeight(34);
    form->addRow("Tipo de Atendimento:", edtTipo);
    form->addRow("Prioridade:", cboPrioridade);

    auto* btnBox = new QHBoxLayout();
    btnNegar   = new QPushButton("✗  Negar");
    btnNegar->setObjectName("btnNegar");
    btnAceitar = new QPushButton("✓  Aceitar e Adicionar à Fila");
    btnAceitar->setObjectName("btnPrim");
    btnBox->addWidget(btnNegar);
    btnBox->addWidget(btnAceitar);

    auto* btnFechar = new QPushButton("Fechar");
    btnFechar->setObjectName("btnSec");

    layout->addWidget(lblTit);
    layout->addWidget(new QLabel("Pacientes com solicitação:"));
    layout->addWidget(lstPacientes);
    layout->addWidget(new QLabel("Detalhes:"));
    layout->addWidget(txtDetalhes);
    layout->addSpacing(4);
    layout->addLayout(form);
    layout->addLayout(btnBox);
    layout->addWidget(btnFechar);

    connect(lstPacientes, &QListWidget::currentRowChanged, this, &SolicitacoesDialog::onSelecionar);
    connect(btnAceitar,   &QPushButton::clicked, this, &SolicitacoesDialog::onAceitar);
    connect(btnNegar,     &QPushButton::clicked, this, &SolicitacoesDialog::onNegar);
    connect(btnFechar,    &QPushButton::clicked, this, &QDialog::accept);

    carregarLista();
    applyStyle();
}

void SolicitacoesDialog::carregarLista() {
    lstPacientes->clear();
    pacientesComSol.clear();
    for (auto u : usuarios) {
        Paciente* p = dynamic_cast<Paciente*>(u);
        if (p && p->possuiSolicitacao()) {
            pacientesComSol.push_back(p);
            lstPacientes->addItem(QString::fromStdString(p->getNome()) + " [" +
                                  QString::fromStdString(p->getLogin()) + "]");
        }
    }
    if (pacientesComSol.empty()) {
        txtDetalhes->setText("Nenhuma solicitação pendente no momento.");
        btnAceitar->setEnabled(false);
        btnNegar->setEnabled(false);
    }
}

void SolicitacoesDialog::onSelecionar(int row) {
    if (row < 0 || row >= (int)pacientesComSol.size()) return;
    Paciente* p = pacientesComSol[row];
    QString info;
    info += "Paciente: " + QString::fromStdString(p->getNome()) + "\n";
    info += "Status: "   + QString::fromStdString(p->getStatus()) + "\n";
    info += "Sintomas: " + QString::fromStdString(p->getDescricaoSolicitacao());
    txtDetalhes->setText(info);
}

void SolicitacoesDialog::onAceitar() {
    int row = lstPacientes->currentRow();
    if (row < 0 || row >= (int)pacientesComSol.size()) {
        QMessageBox::warning(this, "Atenção", "Selecione um paciente."); return;
    }
    Paciente* p = pacientesComSol[row];
    std::string tipo = edtTipo->text().trimmed().toStdString();
    if (tipo.empty()) { QMessageBox::warning(this, "Atenção", "Informe o tipo de atendimento."); return; }

    int prioridade = cboPrioridade->currentIndex() + 1;
    auto* t = new Triagem(p->getDescricaoSolicitacao(), tipo, prioridade);
    p->setTriagem(t);
    fila.adicionarPaciente(p);
    p->adicionarHistorico("Solicitação ACEITA | Tipo: " + tipo + " | Prioridade: " + std::to_string(prioridade));
    p->limparSolicitacao();

    QMessageBox::information(this, "Sucesso", "Paciente adicionado à fila!");
    carregarLista();
}

void SolicitacoesDialog::onNegar() {
    int row = lstPacientes->currentRow();
    if (row < 0 || row >= (int)pacientesComSol.size()) {
        QMessageBox::warning(this, "Atenção", "Selecione um paciente."); return;
    }
    Paciente* p = pacientesComSol[row];
    p->adicionarHistorico("Solicitação NEGADA: " + p->getDescricaoSolicitacao());
    p->limparSolicitacao();
    QMessageBox::information(this, "Negado", "Solicitação negada.");
    carregarLista();
}

void SolicitacoesDialog::applyStyle() {
    setStyleSheet(NASE_GLOBAL_STYLE);
}
