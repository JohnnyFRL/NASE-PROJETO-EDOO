#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <vector>
#include "Usuario.hpp"

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    LoginDialog(std::vector<Usuario*>& usuarios, QWidget* parent = nullptr);
    Usuario* getUsuarioLogado() const;

private slots:
    void onConfirmar();

private:
    QLineEdit*  edtLogin;
    QLineEdit*  edtSenha;
    QPushButton* btnOk;
    QPushButton* btnCancelar;
    QLabel*     lblErro;

    std::vector<Usuario*>& usuarios;
    Usuario* usuarioLogado = nullptr;

    void applyStyle();
};

#endif
