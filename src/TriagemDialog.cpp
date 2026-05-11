#include "NaseStyle.h"
#include "TriagemDialog.h"
#include "Paciente.hpp"
#include "Triagem.hpp"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QMessageBox>

TriagemDialog::TriagemDialog(std::vector<Usuario*>& usuarios, FilaPrioridade& fila, QWidget* parent)
    : QDialog(parent), usuarios(usuarios), fila(fila) {
    setWindowTitle("Realizar Triagem");
    setModal(true);
    setMinimumWidth(400);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(12);

    auto* lblTit = new QLabel("Triagem Inicial do Paciente");
    lblTit->setObjectName("lblTitDlg");
    lblTit->setAlignment(Qt::AlignCenter);

    auto* form = new QFormLayout();
    form->setSpacing(10);

    edtLogin = new QLineEdit();
    edtLogin->setPlaceholderText("Login do paciente");
    edtLogin->setMinimumHeight(34);

    edtSintomas = new QTextEdit();
    edtSintomas->setPlaceholderText("Descreva os sintomas...");
    edtSintomas->setMaximumHeight(100);

    cboPrioridade = new QComboBox();
    cboPrioridade->addItem("1 – Alta");
    cboPrioridade->addItem("2 – Média");
    cboPrioridade->addItem("3 – Baixa");
    cboPrioridade->setMinimumHeight(34);

    form->addRow("Login do Paciente:", edtLogin);
    form->addRow("Sintomas:", edtSintomas);
    form->addRow("Prioridade:", cboPrioridade);

    lblErro = new QLabel("");
    lblErro->setObjectName("lblErro");
    lblErro->setWordWrap(true);
    lblErro->hide();

    auto* btnBox = new QHBoxLayout();
    btnCancelar  = new QPushButton("Cancelar");
    btnCancelar->setObjectName("btnSec");
    btnRealizar  = new QPushButton("Realizar Triagem");
    btnRealizar->setObjectName("btnPrim");
    btnBox->addWidget(btnCancelar);
    btnBox->addWidget(btnRealizar);

    layout->addWidget(lblTit);
    layout->addSpacing(6);
    layout->addLayout(form);
    layout->addWidget(lblErro);
    layout->addSpacing(4);
    layout->addLayout(btnBox);

    connect(btnRealizar, &QPushButton::clicked, this, &TriagemDialog::onRealizar);
    connect(btnCancelar, &QPushButton::clicked, this, &QDialog::reject);

    applyStyle();
}

void TriagemDialog::onRealizar() {
    lblErro->hide();
    std::string login = edtLogin->text().trimmed().toStdString();
    std::string sint  = edtSintomas->toPlainText().trimmed().toStdString();

    if (login.empty()) { lblErro->setText("Informe o login."); lblErro->show(); return; }
    if (sint.empty())  { lblErro->setText("Informe os sintomas."); lblErro->show(); return; }

    for (auto u : usuarios) {
        Paciente* p = dynamic_cast<Paciente*>(u);
        if (p && p->getLogin() == login) {
            if (p->fezTriagem()) {
                lblErro->setText("Paciente já passou pela triagem inicial."); lblErro->show(); return;
            }
            int prioridade = cboPrioridade->currentIndex() + 1;
            auto* t = new Triagem(sint, "Triagem Inicial", prioridade);
            p->setTriagem(t);
            fila.adicionarPaciente(p);
            p->adicionarHistorico("Triagem inicial | Sintomas: " + sint +
                                  " | Prioridade: " + std::to_string(prioridade));
            p->marcarTriagemRealizada();
            p->limparSolicitacao();
            QMessageBox::information(this, "Sucesso", "Triagem realizada e paciente adicionado à fila!");
            accept();
            return;
        }
    }
    lblErro->setText("Paciente não encontrado."); lblErro->show();
}

void TriagemDialog::applyStyle() {
    setStyleSheet(NASE_GLOBAL_STYLE);
}
