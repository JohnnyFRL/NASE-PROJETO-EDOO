#ifndef EDITARDADOSDIALOG_H
#define EDITARDADOSDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>
#include <vector>
#include "Paciente.hpp"
#include "Usuario.hpp"

class EditarDadosDialog : public QDialog {
    Q_OBJECT
public:
    EditarDadosDialog(Paciente* paciente, std::vector<Usuario*>& usuarios, QWidget* parent = nullptr);

private slots:
    void onSalvar();
    void onToggleSenha();

private:
    Paciente*              paciente;
    std::vector<Usuario*>& usuarios;

    //campos editáveis
    QLineEdit* edtNome;
    QLineEdit* edtIdade;
    QLineEdit* edtTelefone;
    QLineEdit* edtEndereco;
    QLineEdit* edtCurso;
    QLineEdit* edtEmail;
    QCheckBox* chkPROAES;

    //ver senha (somente leitura)
    QLineEdit* edtSenhaVer;
    QPushButton* btnToggleSenha;

    QLabel* lblErro;
    QLabel* lblStatus;

    void applyStyle();
    void atualizarStatus();
};

#endif
