#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <vector>
#include "Usuario.hpp"
#include "Funcionario.hpp"
#include "FilaPrioridade.hpp"
#include "Database.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onLoginClicked();
    void onSairClicked();

private:
    void setupUI();

    QLineEdit*   edtLogin;
    QLineEdit*   edtSenha;
    QPushButton* btnLogin;
    QPushButton* btnSair;
    QPushButton* btnVerSenha;
    QLabel*      lblErro;

    Database*             db;
    std::vector<Usuario*> usuarios;
    FilaPrioridade        fila;
};
#endif
