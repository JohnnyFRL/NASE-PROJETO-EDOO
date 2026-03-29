#ifndef PESSOA_HPP
#define PESSOA_HPP

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Pessoa{
private:
    int idade;
    string nome;
    string cpf;
    string telefone;
    string endereco;

public:
    Pessoa(string nome, int idade,string cpf, string telefone, string endereco);
    string getPessoa();
    int getIdade();
};

#endif