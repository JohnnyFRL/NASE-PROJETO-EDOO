#include "NaseStyle.h"
#include "Database.hpp"
#include "CadastroDialog.h"
#include "Paciente.hpp"
#include "Validacao.hpp"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QMessageBox>

CadastroDialog::CadastroDialog(std::vector<Usuario*>& usuarios, Database* db, QWidget* parent)
    : QDialog(parent), usuarios(usuarios), db(db) {
    setWindowTitle("Cadastrar Paciente");
    setModal(true);
    setMinimumWidth(420);
    setMinimumHeight(520);

    auto* scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    auto* container = new QWidget();
    auto* form = new QFormLayout(container);
    form->setSpacing(10);
    form->setContentsMargins(20, 20, 20, 20);

    auto mkEdit = [&](const QString& ph) {
        auto* e = new QLineEdit();
        e->setPlaceholderText(ph);
        e->setMinimumHeight(34);
        return e;
    };

    edtLogin    = mkEdit("mínimo 3 caracteres, sem espaços");
    edtSenha    = mkEdit("mín. 8 chars, maiúsc., núm., símbolo");
    edtSenha->setEchoMode(QLineEdit::Password);
    edtNome     = mkEdit("Nome completo");
    edtIdade    = mkEdit("ex: 21");
    edtCpf      = mkEdit("ex: 123.456.789-00");
    edtTelefone = mkEdit("ex: (81) 91234-5678");
    edtEndereco = mkEdit("Rua, número, bairro...");
    edtCurso    = mkEdit("ex: Sistema de Informação");
    edtEmail    = mkEdit("ex: aluno@ufpe.br");
    chkUFPE     = new QCheckBox("Aluno da UFPE");
    chkPROAES   = new QCheckBox("Bolsista PROAES");

    form->addRow("Login*:",    edtLogin);
    form->addRow("Senha*:",    edtSenha);
    form->addRow("Nome*:",     edtNome);
    form->addRow("Idade*:",    edtIdade);
    form->addRow("CPF*:",      edtCpf);
    form->addRow("Telefone*:", edtTelefone);
    form->addRow("Endereço:",  edtEndereco);
    form->addRow("Curso:",     edtCurso);
    form->addRow("Email*:",    edtEmail);
    form->addRow("",           chkUFPE);
    form->addRow("",           chkPROAES);

    lblErro = new QLabel("", container);
    lblErro->setObjectName("lblErro");
    lblErro->setWordWrap(true);
    lblErro->hide();
    form->addRow(lblErro);

    scroll->setWidget(container);

    auto* btnBox    = new QHBoxLayout();
    btnCancelar     = new QPushButton("Cancelar");
    btnCancelar->setObjectName("btnSec");
    btnCadastrar    = new QPushButton("Cadastrar");
    btnCadastrar->setObjectName("btnPrim");
    btnBox->addWidget(btnCancelar);
    btnBox->addWidget(btnCadastrar);

    auto* root = new QVBoxLayout(this);
    auto* lblTit = new QLabel("Cadastro de Paciente");
    lblTit->setObjectName("lblTitDlg");
    lblTit->setAlignment(Qt::AlignCenter);
    root->addWidget(lblTit);
    root->addWidget(scroll);
    root->addSpacing(6);
    root->addLayout(btnBox);
    root->setContentsMargins(16, 16, 16, 16);

    connect(btnCadastrar, &QPushButton::clicked, this, &CadastroDialog::onCadastrar);
    connect(btnCancelar,  &QPushButton::clicked, this, &QDialog::reject);

    applyStyle();
}

void CadastroDialog::onCadastrar() {
    lblErro->hide();
    std::string erro;

    std::string login    = edtLogin->text().trimmed().toStdString();
    std::string senha    = edtSenha->text().toStdString();
    std::string nome     = edtNome->text().trimmed().toStdString();
    std::string idadeStr = edtIdade->text().trimmed().toStdString();
    std::string cpf      = edtCpf->text().trimmed().toStdString();
    std::string tel      = edtTelefone->text().trimmed().toStdString();
    std::string end      = edtEndereco->text().trimmed().toStdString();
    std::string curso    = edtCurso->text().trimmed().toStdString();
    std::string email    = edtEmail->text().trimmed().toStdString();
    bool ufpe   = chkUFPE->isChecked();
    bool proaes = chkPROAES->isChecked();

    if (!Validacao::validarLoginUnico(login, usuarios, erro)) {
        lblErro->setText(QString::fromStdString(erro)); lblErro->show(); return;
    }
    if (!Validacao::validarSenhaForte(senha, erro)) {
        lblErro->setText(QString::fromStdString(erro)); lblErro->show(); return;
    }
    if (!Validacao::validarNome(nome, erro)) {
        lblErro->setText(QString::fromStdString(erro)); lblErro->show(); return;
    }
    if (idadeStr.empty()) {
        lblErro->setText("Idade não pode ser vazia."); lblErro->show(); return;
    }
    for (char c : idadeStr) {
        if (c < '0' || c > '9') {
            lblErro->setText("Idade deve conter apenas números."); lblErro->show(); return;
        }
    }
    int idade = std::stoi(idadeStr);
    if (idade <= 0 || idade > 130) {
        lblErro->setText("Idade inválida."); lblErro->show(); return;
    }

    std::string numCpf;
    for (char c : cpf) {
        if (c >= '0' && c <= '9') numCpf += c;
        else if (c == '.' || c == '-') continue;
        else { lblErro->setText("CPF inválido."); lblErro->show(); return; }
    }
    if (numCpf.size() != 11) {
        lblErro->setText("CPF deve ter 11 dígitos."); lblErro->show(); return;
    }
    if (!Validacao::validarTelefone(tel, erro)) {
        lblErro->setText(QString::fromStdString(erro)); lblErro->show(); return;
    }
    if (!Validacao::validarEmail(email, erro)) {
        lblErro->setText(QString::fromStdString(erro)); lblErro->show(); return;
    }
    if (!ufpe) {
        lblErro->setText("Apenas alunos da UFPE podem se cadastrar."); lblErro->show(); return;
    }

    auto* p = new Paciente(db, nome, idade, cpf, tel, end, curso, email, ufpe, proaes, login, senha);
    p->validar();
    p->saveToDatabase();
    usuarios.push_back(p);

    QMessageBox::information(this, "Sucesso", "Paciente cadastrado com sucesso!");
    accept();
}

void CadastroDialog::applyStyle() {
    setStyleSheet(NASE_GLOBAL_STYLE);
}
