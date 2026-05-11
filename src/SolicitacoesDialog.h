#ifndef SOLICITACOESDIALOG_H
#define SOLICITACOESDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <vector>
#include "Usuario.hpp"
#include "FilaPrioridade.hpp"

class SolicitacoesDialog : public QDialog {
    Q_OBJECT
public:
    SolicitacoesDialog(std::vector<Usuario*>& usuarios, FilaPrioridade& fila, QWidget* parent = nullptr);

private slots:
    void onAceitar();
    void onNegar();
    void onSelecionar(int row);

private:
    std::vector<Usuario*>& usuarios;
    FilaPrioridade&        fila;
    std::vector<Paciente*> pacientesComSol;

    QListWidget* lstPacientes;
    QTextEdit*   txtDetalhes;
    QComboBox*   cboPrioridade;
    QPushButton* btnAceitar;
    QPushButton* btnNegar;
    QLabel*      lblTipo;
    QLineEdit*   edtTipo;

    void carregarLista();
    void applyStyle();
};

#endif
