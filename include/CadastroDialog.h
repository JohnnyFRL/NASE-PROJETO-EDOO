#ifndef CADASTRODIALOG_H
#define CADASTRODIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <vector>
#include "Usuario.hpp"
#include "Database.hpp"

class CadastroDialog : public QDialog {
    Q_OBJECT
public:
    CadastroDialog(std::vector<Usuario*>& usuarios, Database* db, QWidget* parent = nullptr);

private slots:
    void onCadastrar();

private:
    std::vector<Usuario*>& usuarios;
    Database* db;

    QLineEdit* edtLogin;
    QLineEdit* edtSenha;
    QLineEdit* edtNome;
    QLineEdit* edtIdade;
    QLineEdit* edtCpf;
    QLineEdit* edtTelefone;
    QLineEdit* edtEndereco;
    QLineEdit* edtCurso;
    QLineEdit* edtEmail;
    QCheckBox* chkUFPE;
    QCheckBox* chkPROAES;
    QPushButton* btnCadastrar;
    QPushButton* btnCancelar;
    QLabel* lblErro;

    void applyStyle();
};

#endif
