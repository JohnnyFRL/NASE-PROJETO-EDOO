#include "Paciente.hpp"


Paciente::Paciente(string nome, int idade, string cpf, string telefone, string endereco, string historicoMedico, string curso, string email, bool alunoUFPE, bool bolsistaPROAES,
    string login, string senha)
    : Pessoa(nome, idade, cpf, telefone, endereco), Usuario(login, senha) {
    this->historicoMedico = historicoMedico;
    this->curso = curso;
    this->email = email;
    this->alunoUFPE= alunoUFPE;
    this->bolsistaPROAES= bolsistaPROAES; 
    this->status = NAO_VALIDADO;
    this->triagem = nullptr;
}

string Paciente::getPaciente(){
    string info = "Nome: " + getNome(); // direto da classe Pessoa
     info += " | Login: " + getLogin();
    info += " | Status: " + getStatus();
    if(triagem != nullptr){
        info += " | " + triagem->getResumo();
    }

    return info;
}

void Paciente::validar(){
    if(alunoUFPE && bolsistaPROAES){
        status = VALIDADO;
    } else if(alunoUFPE){
        status = PERDE_PRIORIDADE;
    } else{
        status = NAO_VALIDADO;
    }
}

string Paciente::getStatus(){
    switch(status){
        case VALIDADO: return "Validado";
        case NAO_VALIDADO: return "Nao validado";
        case PERDE_PRIORIDADE: return "Perde prioridade";
        default: return "Desconhecido";
    }
}

StatusPaciente Paciente::getStatusEnum(){
    return status;
}

void Paciente::menu(vector<Usuario*>& usuarios, FilaPrioridade& fila){
    cout << "\n--- MENU PACIENTE ---" << endl;
}

Triagem* Paciente::getTriagem(){
    return triagem;
}

void Paciente::setTriagem(Triagem* t){
    this->triagem = t;
}

bool Paciente::isAlunoUFPE(){
    return alunoUFPE;
}

bool Paciente::isBolsistaPROAES(){
    return bolsistaPROAES;
}