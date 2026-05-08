#include "Pessoa.hpp"

using namespace std;

Pessoa::Pessoa(const string& nome, int idade, const string& cpf, const string& telefone, const string& endereco)
    : id(-1), idade(idade), nome(nome), cpf(cpf), telefone(telefone), endereco(endereco) {
}

Pessoa::Pessoa(int id, const string& nome, int idade, const string& cpf, const string& telefone, const string& endereco)
    : id(id), idade(idade), nome(nome), cpf(cpf), telefone(telefone), endereco(endereco) {
}

string Pessoa::getPessoa() const {
    return "nome: " + nome + ", cpf: " + cpf + ", telefone: " + telefone + ", endereco: " + endereco;
}

int Pessoa::getId() const {
    return id;
}

void Pessoa::setId(int novoId) {
    id = novoId;
}

int Pessoa::getIdade() const {
    return idade;
}

const string& Pessoa::getNome() const {
    return nome;
}

const string& Pessoa::getCpf() const {
    return cpf;
}

const string& Pessoa::getTelefone() const {
    return telefone;
}

const string& Pessoa::getEndereco() const {
    return endereco;
}

void Pessoa::setNome(const string& novoNome) {
    nome = novoNome;
}

void Pessoa::setIdade(int novaIdade) {
    idade = novaIdade;
}

void Pessoa::setTelefone(const string& novoTelefone) {
    telefone = novoTelefone;
}

void Pessoa::setEndereco(const string& novoEndereco) {
    endereco = novoEndereco;
}