#include <iostream>
#include <vector>
#include <string>

#ifndef USUARIO_HPP
#define USUARIO_HPP
using namespace std;
class FilaPrioridade;

class Usuario {
private:
    string login;
    string senha;

public:
    Usuario(string login, string senha);
    string getLogin();
    bool autenticar(string senha);
    virtual void menu(vector<Usuario*>& usuarios, FilaPrioridade& fila) = 0; // pra ser usado nas classes filhas de forma obrigatoria
    virtual ~Usuario() {} // garantir limpeza de memoria para classes filhas
};
#endif