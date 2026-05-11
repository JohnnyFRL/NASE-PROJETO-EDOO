#ifndef MENUPACIENTEWINDOW_H
#define MENUPACIENTEWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <vector>
#include "Paciente.hpp"
#include "FilaPrioridade.hpp"
#include "Database.hpp"

class MenuPacienteWindow : public QWidget {
    Q_OBJECT
public:
    MenuPacienteWindow(Paciente* paciente, std::vector<Usuario*>& usuarios,
                       FilaPrioridade& fila, Database* db, QWidget* parent = nullptr);
private slots:
    void onSolicitarConsulta();
    void onVerHistorico();
    void onVerDados();
    void onVoltar();
private:
    Paciente*              paciente;
    std::vector<Usuario*>& usuarios;
    FilaPrioridade&        fila;
    Database*              db;
    QLabel*    lblTitulo;
    QLabel*    lblBemVindo;
    QLabel*    lblStatus;
    QTextEdit* txtInfo;
    void applyStyle();
    void atualizarStatus();
};
#endif
