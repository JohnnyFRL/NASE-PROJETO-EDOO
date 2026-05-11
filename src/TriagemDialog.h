#ifndef TRIAGEMDIALOG_H
#define TRIAGEMDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <vector>
#include "Usuario.hpp"
#include "FilaPrioridade.hpp"

class TriagemDialog : public QDialog {
    Q_OBJECT
public:
    TriagemDialog(std::vector<Usuario*>& usuarios, FilaPrioridade& fila, QWidget* parent = nullptr);

private slots:
    void onRealizar();

private:
    std::vector<Usuario*>& usuarios;
    FilaPrioridade&        fila;

    QLineEdit* edtLogin;
    QTextEdit* edtSintomas;
    QComboBox* cboPrioridade;
    QPushButton* btnRealizar;
    QPushButton* btnCancelar;
    QLabel* lblErro;

    void applyStyle();
};

#endif
