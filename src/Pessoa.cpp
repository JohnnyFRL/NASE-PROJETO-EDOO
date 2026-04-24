#include "Pessoa.hpp"

Pessoa::Pessoa(string nome, int idade,string cpf, string telefone, string endereco){
    this->nome = nome;
    this->idade = idade;
    this->cpf = cpf;
    this->telefone = telefone;
    this->endereco = endereco;
}

string Pessoa::getPessoa(){
    return "nome: " + nome + ", cpf: " + cpf + ", telefone: " + telefone + ", endereco: " + endereco;
}

int Pessoa::getIdade(){
    return idade;
}

string Pessoa::getNome(){
    return nome;
}

string Pessoa::getCpf(){
    return cpf;
}