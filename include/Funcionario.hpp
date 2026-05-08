#ifndef FUNCIONARIO_HPP
#define FUNCIONARIO_HPP 

#include <iostream>
#include <vector>
#include "Usuario.hpp"
#include "FilaPrioridade.hpp"
#include "Database.hpp"

using namespace std;

class Funcionario : public Usuario{
private:
    Database* db;

public:
    Funcionario(Database* db, string login, string senha);
    void menu(vector<Usuario*>& usuarios, FilaPrioridade& fila) override;
};

#endif