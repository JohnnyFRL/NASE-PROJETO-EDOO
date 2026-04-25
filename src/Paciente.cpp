#include "FilaPrioridade.hpp"
#include "Paciente.hpp"

Paciente::Paciente(string nome, int idade, string cpf, string telefone, string endereco, string curso, string email, bool alunoUFPE, bool bolsistaPROAES,
string login, string senha)
: Pessoa(nome, idade, cpf, telefone, endereco), Usuario(login, senha) {
    this->curso = curso;
    this->email = email;
    this->alunoUFPE= alunoUFPE;
    this->bolsistaPROAES= bolsistaPROAES;
    this->status = NAO_VALIDADO;
    this->triagem = nullptr;
    this->temSolicitacao = false;
    this->emFila = false;
    this->jaFezTriagem = false;
}

// ATUALIZADO: Saida para o Funcionario
string Paciente::getPaciente(){
    string info = "\n--- HISTORICO: " + getNome() + " (Login: " + getLogin() + ") ---\n";
    info += "Status: " + getStatus() + "\n\n";
    info += "[ REGISTROS ]\n";

    if(triagem != nullptr){
        string resumo = triagem->getResumo();
        
        
        size_t posTipo = resumo.find(" | Tipo");   // Limpa a string cortando a parte do " | Tipo: Triagem Inicial" que vinha grudada
        if(posTipo != string::npos) {
            resumo = resumo.substr(0, posTipo);
        }
        
        int p = triagem->getPrioridade();
        if(p == 1) resumo += " (Alta)";
        else if(p == 2) resumo += " (Média)";
        else if(p == 3) resumo += " (Baixa)";

        info += "> Triagem Inicial\n";
        info += "  " + resumo + "\n";
    }

    if(historicoSolicitacoes.empty() && triagem == nullptr){
        info += "Nenhum registro encontrado.\n";
    } else {
        for(size_t i = 0; i < historicoSolicitacoes.size(); i++){
            if(historicoSolicitacoes[i].find("Triagem inicial") == string::npos && 
               historicoSolicitacoes[i].find("Tipo:") == string::npos){
                info += "> " + historicoSolicitacoes[i] + "\n";
            }
        }
    }
    
    info += "---------------------------------------------------------\n";
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
        cout << "2. Ver historico\n";
        cout << "3. Ver meus dados\n";
        cout << "0. Sair\n";
        cout << "Escolha: ";
        cin >> opcao;

        switch(opcao){
            case 1:
                solicitarConsulta();
                break;

            case 2:
                mostrarHistorico();
                break;
            case 3:
                cout << getDadosBasicos() << endl;
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
    if(emFila){
        cout << "voce esta na fila de atendimento.\n";
        return;
    }
    if(!jaFezTriagem){
        cout << "Voce precisa passar pela triagem inicial primeiro.\n";
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
    descricaoSolicitacao = ""; // evitar lixo de dados
}

void Paciente::adicionarHistorico(string registro){
    historicoSolicitacoes.push_back(registro);
}

vector<string> Paciente::getHistoricoSolicitacoes(){
    return historicoSolicitacoes;
}

void Paciente::finalizarAtendimento(){
    temSolicitacao = false;
    descricaoSolicitacao = "";

    adicionarHistorico(
        "Atendimento finalizado | Ultima prioridade: " +
        to_string(triagem->getPrioridade())
    );

    // NÃO apaga triagem → mantém histórico
}

// ATUALIZADO: Saida para o proprio Paciente no menu dele
void Paciente::mostrarHistorico(){
    cout << "\n--- HISTORICO: " << getNome() << " (Login: " << getLogin() << ") ---\n";
    cout << "Status: " << getStatus() << "\n\n";
    
    cout << "[ REGISTROS ]\n";

    if(triagem != nullptr){
        string resumo = triagem->getResumo();
        
        size_t posTipo = resumo.find(" | Tipo");
        if(posTipo != string::npos) {
            resumo = resumo.substr(0, posTipo);
        }
        
        int p = triagem->getPrioridade();
        if(p == 1) resumo += " (Alta)";
        else if(p == 2) resumo += " (Média)";
        else if(p == 3) resumo += " (Baixa)";

        cout << "> Triagem Inicial\n";
        cout << "  " << resumo << "\n";
    }

    if(historicoSolicitacoes.empty() && triagem == nullptr){
        cout << "Nenhum registro encontrado.\n";
    } else {
        for(size_t i = 0; i < historicoSolicitacoes.size(); i++){
            if(historicoSolicitacoes[i].find("Triagem inicial") == string::npos && 
               historicoSolicitacoes[i].find("Tipo:") == string::npos){
                cout << "> " << historicoSolicitacoes[i] << "\n";
            }
        }
    }
    
    cout << "---------------------------------------------------------\n\n";
}

bool Paciente::estaNaFila(){
    return emFila;
}

void Paciente::setEmFila(bool valor){
    emFila = valor;
}

string Paciente::getDadosBasicos(){
    string info = "Nome: " + getNome();
    info += "\nIdade: " + to_string(getIdade());
    info += "\nCPF: " + getCpf();
    info += "\nCurso: " + curso;
    info += "\nEmail: " + email;
    info += "\nStatus: " + getStatus();

    return info;
}