#ifndef FUNCIONARIO_HPP
#define FUNCIONARIO_HPP 

#include <iostream>
#include <vector>
#include "Usuario.hpp"
#include "FilaPrioridade.hpp"

using namespace std;

class Funcionario : public Usuario{
public:
    Funcionario(string login, string senha);
    void menu(vector<Usuario*>& usuarios, FilaPrioridade& fila) override;
};

#endif