#include "NaseStyle.h"
#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

LoginDialog::LoginDialog(std::vector<Usuario*>& usuarios, QWidget* parent)
    : QDialog(parent), usuarios(usuarios) {
    setWindowTitle("Login");
    setModal(true);
    setMinimumWidth(340);

    auto* layout = new QVBoxLayout(this);
    layout->setSpacing(14);
    layout->setContentsMargins(30, 28, 30, 28);

    auto* lblTit = new QLabel("Acesso ao Sistema", this);
    lblTit->setAlignment(Qt::AlignCenter);
    lblTit->setObjectName("lblTitDlg");

    auto* form = new QFormLayout();
    form->setSpacing(10);

    edtLogin = new QLineEdit(this);
    edtLogin->setPlaceholderText("seu login");
    edtLogin->setMinimumHeight(36);

    edtSenha = new QLineEdit(this);
    edtSenha->setPlaceholderText("sua senha");
    edtSenha->setEchoMode(QLineEdit::Password);
    edtSenha->setMinimumHeight(36);

    form->addRow("Login:", edtLogin);
    form->addRow("Senha:", edtSenha);

    lblErro = new QLabel("", this);
    lblErro->setObjectName("lblErro");
    lblErro->setAlignment(Qt::AlignCenter);
    lblErro->hide();

    auto* btnBox = new QHBoxLayout();
    btnCancelar = new QPushButton("Cancelar", this);
    btnCancelar->setObjectName("btnSec");
    btnOk = new QPushButton("Entrar", this);
    btnOk->setObjectName("btnPrim");
    btnOk->setDefault(true);
    btnBox->addWidget(btnCancelar);
    btnBox->addWidget(btnOk);

    layout->addWidget(lblTit);
    layout->addSpacing(6);
    layout->addLayout(form);
    layout->addWidget(lblErro);
    layout->addSpacing(4);
    layout->addLayout(btnBox);

    connect(btnOk,       &QPushButton::clicked, this, &LoginDialog::onConfirmar);
    connect(btnCancelar, &QPushButton::clicked, this, &QDialog::reject);
    connect(edtSenha, &QLineEdit::returnPressed, this, &LoginDialog::onConfirmar);

    applyStyle();
}

Usuario* LoginDialog::getUsuarioLogado() const { return usuarioLogado; }

void LoginDialog::onConfirmar() {
    QString login = edtLogin->text().trimmed();
    QString senha = edtSenha->text();

    for (auto u : usuarios) {
        if (u->getLogin() == login.toStdString() && u->autenticar(senha.toStdString())) {
            usuarioLogado = u;
            accept();
            return;
        }
    }
    lblErro->setText("Login ou senha inválidos.");
    lblErro->show();
    edtSenha->clear();
    edtSenha->setFocus();
}

void LoginDialog::applyStyle() {
    setStyleSheet(NASE_GLOBAL_STYLE);
}
