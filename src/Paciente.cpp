#include "Paciente.hpp"

Paciente::Paciente(string nome, int idade, string cpf, string telefone, string endereco, string historicoMedico, string curso, string email, bool alunoUFPE, bool bolsistaPROAES)
    : Pessoa(nome, idade, cpf, telefone, endereco) {
    this->historicoMedico = historicoMedico;
    this->curso = curso;
    this->email = email;
    this->alunoUFPE = false;
    this->bolsistaPROAES = false; 
    this->status = "em analise";
}

string Paciente::getPaciente() {
    return getPessoa() + ", historicoMedico: " + historicoMedico + ", curso: " + curso + ", email: " + email;
}

void Paciente::validar(){
    if(alunoUFPE && bolsistaPROAES == true){
        this->status = "Validado";
    } else if(alunoUFPE == true && bolsistaPROAES == false){
        this->status = "apto apenas Acolhimento";
    } else {
        this->status = "Nao Validado";
    }
}

string Paciente::getStatus() {
    return this->status;
}