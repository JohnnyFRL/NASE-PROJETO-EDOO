#include "NaseStyle.h"
#include "EditarDadosDialog.h"
#include "Validacao.hpp"
#include "Usuario.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QMessageBox>

EditarDadosDialog::EditarDadosDialog(Paciente* paciente, std::vector<Usuario*>& usuarios, QWidget* parent)
    : QDialog(parent), paciente(paciente), usuarios(usuarios) {
    setWindowTitle("Editar Dados – " + QString::fromStdString(paciente->getNome()));
    setModal(true);
    setMinimumWidth(440);
    setMinimumHeight(540);

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(20, 20, 20, 20);
    root->setSpacing(12);

    auto* lblTit = new QLabel("✏️  Editar Dados do Paciente", this);
    lblTit->setObjectName("lblTitDlg");
    lblTit->setAlignment(Qt::AlignCenter);

    lblStatus = new QLabel("", this);
    lblStatus->setObjectName("lblStatusDlg");
    lblStatus->setAlignment(Qt::AlignCenter);
    atualizarStatus();

    auto* scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    auto* container = new QWidget();
    auto* form = new QFormLayout(container);
    form->setSpacing(10);
    form->setContentsMargins(10, 10, 10, 10);

    auto mkEdit = [&](const QString& valor, const QString& ph = "") {
        auto* e = new QLineEdit(valor);
        e->setPlaceholderText(ph);
        e->setMinimumHeight(36);
        return e;
    };

    edtNome      = mkEdit(QString::fromStdString(paciente->getNome()));
    edtIdade     = mkEdit(QString::number(paciente->getIdade()));
    edtTelefone  = mkEdit(QString::fromStdString(paciente->getTelefone()), "(XX) 9XXXX-XXXX");
    edtEndereco  = mkEdit(QString::fromStdString(paciente->getEndereco()));
    edtCurso     = mkEdit("", "Curso");  // getter não existia antes — campo livre
    edtEmail     = mkEdit("", "email@ufpe.br");
    chkPROAES    = new QCheckBox("Bolsista PROAES");
    chkPROAES->setChecked(paciente->isBolsistaPROAES());

    form->addRow("Nome completo:", edtNome);
    form->addRow("Idade:",         edtIdade);
    form->addRow("Telefone:",      edtTelefone);
    form->addRow("Endereço:",      edtEndereco);
    form->addRow("Curso:",         edtCurso);
    form->addRow("Email:",         edtEmail);
    form->addRow("",               chkPROAES);

    auto* grpSenha = new QGroupBox("🔐  Senha do Paciente", container);
    grpSenha->setObjectName("grpSenha");
    auto* senhaLay = new QHBoxLayout(grpSenha);
    senhaLay->setSpacing(8);

    edtSenhaVer = new QLineEdit(grpSenha);
    edtSenhaVer->setEchoMode(QLineEdit::Password);
    edtSenhaVer->setReadOnly(true);
    edtSenhaVer->setMinimumHeight(36);
    edtSenhaVer->setPlaceholderText("Clique no olho para revelar");

    edtSenhaVer->setText(QString::fromStdString(paciente->getSenha()));

    btnToggleSenha = new QPushButton("👁", grpSenha);
    btnToggleSenha->setObjectName("btnOlho");
    btnToggleSenha->setFixedSize(36, 36);
    btnToggleSenha->setCursor(Qt::PointingHandCursor);
    btnToggleSenha->setCheckable(true);

    senhaLay->addWidget(edtSenhaVer);
    senhaLay->addWidget(btnToggleSenha);

    form->addRow(grpSenha);

    lblErro = new QLabel("", container);
    lblErro->setObjectName("lblErro");
    lblErro->setWordWrap(true);
    lblErro->hide();
    form->addRow(lblErro);

    scroll->setWidget(container);

    auto* btnBox    = new QHBoxLayout();
    auto* btnCancel = new QPushButton("Cancelar", this);
    btnCancel->setObjectName("btnSec");
    auto* btnSalvar = new QPushButton("💾  Salvar Alterações", this);
    btnSalvar->setObjectName("btnPrim");
    btnSalvar->setMinimumHeight(40);
    btnBox->addWidget(btnCancel);
    btnBox->addWidget(btnSalvar);

    root->addWidget(lblTit);
    root->addWidget(lblStatus);
    root->addWidget(scroll);
    root->addLayout(btnBox);

    connect(btnSalvar,      &QPushButton::clicked, this, &EditarDadosDialog::onSalvar);
    connect(btnCancel,      &QPushButton::clicked, this, &QDialog::reject);
    connect(btnToggleSenha, &QPushButton::toggled, this, &EditarDadosDialog::onToggleSenha);

    applyStyle();
}

void EditarDadosDialog::atualizarStatus() {
    QString s = QString::fromStdString(paciente->getStatus());
    QString cor = "#f87171";
    if (paciente->getStatusEnum() == VALIDADO)          cor = "#34d399";
    else if (paciente->getStatusEnum() == PERDE_PRIORIDADE) cor = "#fbbf24";
    lblStatus->setText("Status atual: <span style='color:" + cor + ";font-weight:bold'>" + s + "</span>");
    lblStatus->setTextFormat(Qt::RichText);
}

void EditarDadosDialog::onToggleSenha() {
    if (btnToggleSenha->isChecked()) {
        edtSenhaVer->setEchoMode(QLineEdit::Normal);
        btnToggleSenha->setText("🙈");
    } else {
        edtSenhaVer->setEchoMode(QLineEdit::Password);
        btnToggleSenha->setText("👁");
    }
}

void EditarDadosDialog::onSalvar() {
    lblErro->hide();
    std::string erro;

    std::string nome = edtNome->text().trimmed().toStdString();
    if (!Validacao::validarNome(nome, erro)) {
        lblErro->setText(QString::fromStdString(erro)); lblErro->show(); return;
    }

    std::string idadeStr = edtIdade->text().trimmed().toStdString();
    for (char c : idadeStr) {
        if (c < '0' || c > '9') {
            lblErro->setText("Idade deve conter apenas números."); lblErro->show(); return;
        }
    }
    int idade = idadeStr.empty() ? 0 : std::stoi(idadeStr);
    if (idade <= 0 || idade > 130) {
        lblErro->setText("Idade inválida."); lblErro->show(); return;
    }

    std::string tel = edtTelefone->text().trimmed().toStdString();
    if (!tel.empty() && !Validacao::validarTelefone(tel, erro)) {
        lblErro->setText(QString::fromStdString(erro)); lblErro->show(); return;
    }

    std::string email = edtEmail->text().trimmed().toStdString();
    if (!email.empty() && !Validacao::validarEmail(email, erro)) {
        lblErro->setText(QString::fromStdString(erro)); lblErro->show(); return;
    }

    paciente->setNome(nome);
    paciente->setIdade(idade);
    if (!tel.empty())   paciente->setTelefone(tel);
    std::string end = edtEndereco->text().trimmed().toStdString();
    if (!end.empty())   paciente->setEndereco(end);
    std::string curso = edtCurso->text().trimmed().toStdString();
    if (!curso.empty()) paciente->setCurso(curso);
    if (!email.empty()) paciente->setEmail(email);
    paciente->setBolsistaPROAES(chkPROAES->isChecked());

    paciente->adicionarHistorico("Dados editados pelo atendente.");
    paciente->updateInDatabase();
    atualizarStatus();

    QMessageBox::information(this, "Sucesso", "Dados atualizados com sucesso!");
    accept();
}

void EditarDadosDialog::applyStyle() {
    setStyleSheet(NASE_GLOBAL_STYLE);
}
