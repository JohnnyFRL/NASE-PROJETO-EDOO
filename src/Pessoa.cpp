#include "Pessoa.hpp"

Pessoa::Pessoa(string nome, int idade) {
    this->nome = nome;
    this->idade = idade;
}

string Pessoa::getNome() const {
    return nome;
}

int Pessoa::getIdade() const {
    return idade;
}