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

string Pessoa::getTelefone(){
    return telefone;
}

string Pessoa::getEndereco(){
    return endereco;
}

void Pessoa::setNome(string novoNome){
    nome = novoNome;
}

void Pessoa::setIdade(int novaIdade){
    idade = novaIdade;
}

void Pessoa::setTelefone(string novoTelefone){
    telefone = novoTelefone;
}

void Pessoa::setEndereco(string novoEndereco){
    endereco = novoEndereco;
}