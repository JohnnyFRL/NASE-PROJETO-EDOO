#include "FilaPrioridade.hpp"
#include "Paciente.hpp"
#include "Validacao.hpp"


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

string Paciente::getPaciente(){
    string info = "Nome: " + getNome(); // direto da classe Pessoa
     info += " | Login: " + getLogin();
    info += " | Status: " + getStatus();
    if(triagem != nullptr){
        info += " | " + triagem->getResumo();
    }
    for(string h : historicoSolicitacoes){
    info += "\n  - " + h;
}

    return info;
}

// Destrutor: libera a memoria alocada dinamicamente para a triagem
Paciente::~Paciente(){
    delete triagem;
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
    if(!fezTriagem()){
    cout << "Voce precisa passar pela triagem inicial primeiro.\n";
    return;
} // pra não duplicar a triagem, já que o paciente só pode solicitar consulta depois de ter sido triado, ou seja, ter uma triagem associada a ele
    if(possuiSolicitacao()){
    cout << "Voce ja possui uma solicitacao em andamento.\n";
    return; // não pode ter outra solicitacao
}
    string sintomas;
    cin.ignore();
    cout << "Descreva seus sintomas: ";
    getline(cin, sintomas);

    criarSolicitacao(sintomas);
    cout << "Solicitacao enviada! Aguarde avaliacao.\n";
}

Triagem* Paciente::getTriagem(){
    return triagem;
}

void Paciente::setTriagem(Triagem* t){
    delete triagem; // evita vazamento ao substituir triagem existente
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
    limparSolicitacao();

    adicionarHistorico(
        "Atendimento finalizado | Ultima prioridade: " + 
        to_string(triagem->getPrioridade())
    );

    // NÃO apaga triagem → mantém histórico
}

void Paciente::mostrarHistorico(){
    cout << "\n--- HISTORICO DO PACIENTE ---\n";
    if(historicoSolicitacoes.empty()){
        cout << "Nenhum registro encontrado.\n";
        return;
    }
    for(string h : historicoSolicitacoes){
        cout << "- " << h << endl;
    }
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
    info += "\nTelefone: " + getTelefone();
    info += "\nEndereco: " + getEndereco();
    info += "\nCurso: " + curso;
    info += "\nEmail: " + email;
    info += "\nStatus: " + getStatus();

    return info;
}

void Paciente::setCurso(string novoCurso){
    curso = novoCurso;
}

void Paciente::setEmail(string novoEmail){
    email = novoEmail;
}

void Paciente::setBolsistaPROAES(bool valor){
    bolsistaPROAES = valor;
    validar();
}


void Paciente::editarDados(vector<Usuario*>& usuarios){
    int opcao;
    string erro, novoValor;

    do {
        cout << "\n--- EDITAR DADOS: " << getNome() << " ---\n";
        cout << getDadosBasicos() << "\n";
        cout << "\n1. Nome\n";
        cout << "2. Idade\n";
        cout << "3. Telefone\n";
        cout << "4. Endereco\n";
        cout << "5. Curso\n";
        cout << "6. Email\n";
        cout << "7. Bolsista PROAES\n";
        cout << "0. Voltar\n";
        cout << "Escolha: ";
        cin >> opcao;
        cin.ignore();

        switch(opcao){
            case 1:
                while(true){
                    cout << "Novo nome: ";
                    getline(cin, novoValor);
                    if(Validacao::validarNome(novoValor, erro)){
                        setNome(novoValor);
                        cout << "Nome atualizado.\n";
                        break;
                    }
                    cout << "[ERRO] " << erro << "\n";
                }
                break;

            case 2: {
                string idadeStr;
                while(true){
                    cout << "Nova idade: ";
                    getline(cin, idadeStr);
                    bool soNumero = true;
                    for(int i = 0; i < (int)idadeStr.length(); i++){
                        if(idadeStr[i] < '0' || idadeStr[i] > '9'){
                            soNumero = false;
                            break;
                        }
                    }
                    if(!soNumero || idadeStr.empty()){
                        cout << "[ERRO] Digite apenas numeros!\n";
                        continue;
                    }
                    int novaIdade = stoi(idadeStr);
                    if(novaIdade <= 0 || novaIdade > 130){
                        cout << "[ERRO] Idade invalida!\n";
                        continue;
                    }
                    setIdade(novaIdade);
                    cout << "Idade atualizada.\n";
                    break;
                }
                break;
            }

            case 3:
                while(true){
                    cout << "Novo telefone: ";
                    getline(cin, novoValor);
                    if(Validacao::validarTelefone(novoValor, erro)){
                        setTelefone(novoValor);
                        cout << "Telefone atualizado.\n";
                        break;
                    }
                    cout << "[ERRO] " << erro << "\n";
                }
                break;

            case 4:
                cout << "Novo endereco: ";
                getline(cin, novoValor);
                setEndereco(novoValor);
                cout << "Endereco atualizado.\n";
                break;

            case 5:
                cout << "Novo curso: ";
                getline(cin, novoValor);
                setCurso(novoValor);
                cout << "Curso atualizado.\n";
                break;

            case 6:
                while(true){
                    cout << "Novo email: ";
                    getline(cin, novoValor);
                    if(Validacao::validarEmail(novoValor, erro)){
                        setEmail(novoValor);
                        cout << "Email atualizado.\n";
                        break;
                    }
                    cout << "[ERRO] " << erro << "\n";
                }
                break;

            case 7: {
                char resp;
                cout << "E bolsista PROAES? (s/n): ";
                cin >> resp;
                cin.ignore();
                setBolsistaPROAES(resp == 's' || resp == 'S');
                cout << "Status atualizado para: " << getStatus() << "\n";
                break;
            }

            case 0:
                cout << "Voltando...\n";
                break;

            default:
                cout << "Opcao invalida!\n";
        }

    } while(opcao != 0);
}

void Paciente::setDescricaoSolicitacao(string desc){
    descricaoSolicitacao = desc;
}

bool Paciente::fezTriagem(){
    return jaFezTriagem;
}

void Paciente::criarSolicitacao(string descricao){
    this->descricaoSolicitacao = descricao;
    this->temSolicitacao = true;
}

void Paciente::marcarTriagemRealizada(){
    jaFezTriagem = true;

}

