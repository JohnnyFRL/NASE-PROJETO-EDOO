#include "Paciente.hpp"


Paciente::Paciente(string nome, int idade, string cpf, string telefone, string endereco, string historicoMedico, string curso, string email, bool alunoUFPE, bool bolsistaPROAES,
    string login, string senha)
    : Pessoa(nome, idade, cpf, telefone, endereco), Usuario(login, senha) {
    this->historicoMedico = historicoMedico;
    this->curso = curso;
    this->email = email;
    this->alunoUFPE= alunoUFPE;
    this->bolsistaPROAES= bolsistaPROAES; 
    this->status = "em analise";
    this->triagem = nullptr;
}

string Paciente::getPaciente(){
    string info = "Nome: " + getNome(); // direto da classe Pessoa
     info += " | Login: " + getLogin();
     info += " | Status: " + status;
    if(triagem != nullptr){
        info += " | " + triagem->getResumo();
    }

    return info;
}

void Paciente::validar(){
    if(alunoUFPE && bolsistaPROAES == true){
        this->status = "Validado";
    } else if(alunoUFPE == true && bolsistaPROAES == false){
        this->status = "apto porem perde priodade de fila";
    } else {
        this->status = "Nao Validado";
    }
}

string Paciente::getStatus() {
    return this->status;
}

void Paciente::menu(vector<Usuario*>& usuarios){
    cout << "\n--- MENU PACIENTE ---" << endl;
}

Triagem* Paciente::getTriagem(){
    return triagem;
}

void Paciente::setTriagem(Triagem* t){
    this->triagem = t;
}