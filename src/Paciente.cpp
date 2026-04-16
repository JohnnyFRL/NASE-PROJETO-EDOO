#include "FilaPrioridade.hpp"
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
    int opcao;

    do{
        cout << "\n--- MENU PACIENTE ---\n";
        cout << "1. Solicitar consulta\n";
        cout << "2. Ver meus dados\n";
        cout << "0. Sair\n";
        cout << "Escolha: ";
        cin >> opcao;

        switch(opcao){
            case 1:
                solicitarConsulta();
                break;

            case 2:
                cout << getPaciente() << endl;
                break;

            case 0:
                cout << "Saindo...\n";
                break;

            default:
                cout << "Opcao invalida!\n";
        }

    }while(opcao != 0); 
}
void Paciente::solicitarConsulta(){
    if(triagem == nullptr){
        cout << "Voce precisa passar pela triagem inicial antes de solicitar consulta.\n";
        return;
    } // pra não duplicar a triagem, já que o paciente só pode solicitar consulta depois de ter sido triado, ou seja, ter uma triagem associada a ele
    if(temSolicitacao){
    cout << "Voce ja possui uma solicitacao em andamento.\n";
    return; // não pode ter outra solicitacao
}
    string sintomas;
    cin.ignore();
    cout << "Descreva seus sintomas: ";
    getline(cin, sintomas);

    this->descricaoSolicitacao = sintomas;
    this->temSolicitacao = true;
    cout << "Solicitacao enviada! Aguarde avaliacao.\n";
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

string Paciente::getDescricaoSolicitacao(){
    return descricaoSolicitacao;
}

bool Paciente::possuiSolicitacao(){
    return temSolicitacao;
}

void Paciente::limparSolicitacao(){
    temSolicitacao = false;
}