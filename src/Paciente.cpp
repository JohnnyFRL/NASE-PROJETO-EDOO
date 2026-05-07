#include "FilaPrioridade.hpp"
#include "Paciente.hpp"
#include "Validacao.hpp"
#include <map>

using namespace std;


Paciente::Paciente(Database* db, string nome, int idade, string cpf, string telefone, string endereco, string curso, string email, bool alunoUFPE, bool bolsistaPROAES,
    string login, string senha)
    : Pessoa(nome, idade, cpf, telefone, endereco), Usuario(login, senha) {
    this->db = db;
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

Paciente::Paciente(Database* db, int idpessoa, string nome, int idade, string cpf, string telefone, string endereco,
             string curso, string email, bool alunoUFPE, bool bolsistaPROAES, string login, string senha,
             StatusPaciente status, bool jaFezTriagem, bool temSolicitacao, bool emFila)
    : Pessoa(idpessoa, nome, idade, cpf, telefone, endereco), Usuario(login, senha) {
    this->db = db;
    this->curso = curso;
    this->email = email;
    this->alunoUFPE = alunoUFPE;
    this->bolsistaPROAES = bolsistaPROAES;
    this->status = status;
    this->triagem = nullptr;
    this->temSolicitacao = temSolicitacao;
    this->emFila = emFila;
    this->jaFezTriagem = jaFezTriagem;
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
    if (db) {
        db->saveHistory(getId(), registro);
    }
}

vector<string> Paciente::getHistoricoSolicitacoes(){
    return historicoSolicitacoes;
}

void Paciente::finalizarAtendimento(){
    limparSolicitacao();
    setEmFila(false); // paciente sai da fila

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
    bool changed = false;
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
                        changed = true;
                        break;
                    }
                    cout << "[ERRO] " << erro << "\n";
                }
                break;

            case 2: {
                string idadeStr;
                int novaIdade;
                while(true){
                    cout << "Nova idade: ";
                    getline(cin, idadeStr);
                    if(Validacao::validarIdade(idadeStr, novaIdade, erro)){
                        setIdade(novaIdade);
                        cout << "Idade atualizada.\n";
                        changed = true;
                        break;
                    }
                    cout << "[ERRO]" << erro << "\n";
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
                        changed = true;
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
                changed = true;
                break;

            case 5:
                cout << "Novo curso: ";
                getline(cin, novoValor);
                setCurso(novoValor);
                cout << "Curso atualizado.\n";
                changed = true;
                break;

            case 6:
                while(true){
                    cout << "Novo email: ";
                    getline(cin, novoValor);
                    if(Validacao::validarEmail(novoValor, erro)){
                        setEmail(novoValor);
                        cout << "Email atualizado.\n";
                        changed = true;
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
                changed = true;
                break;
            }

            case 0:
                cout << "Voltando...\n";
                break;

            default:
                cout << "Opcao invalida!\n";
        }

        if (changed) {
            if (!updateInDatabase()) {
                cout << "[ERRO] Nao foi possivel salvar as alteracoes no banco de dados.\n";
            } else {
                cout << "Alteracoes salvas no banco de dados com sucesso.\n";
            }
            changed = false;
        }

    } while(opcao != 0);
}

bool Paciente::saveToDatabase() {
    // Salvar dados da pessoa primeiro
    int idpessoa = db->savePerson(getId(), getNome(), getIdade(), getCpf(), getTelefone(), getEndereco());
    if (idpessoa == -1) {
        return false;
    }

    setId(idpessoa);

    // Salvar dados do usuário com login real
    if (!db->saveUser(getLogin(), getSenha(), "paciente")) {
        return false;
    }

    // Salvar dados do paciente com o login associado
    string statusStr;
    switch(status) {
        case VALIDADO: statusStr = "VALIDADO"; break;
        case NAO_VALIDADO: statusStr = "NAO_VALIDADO"; break;
        case PERDE_PRIORIDADE: statusStr = "PERDE_PRIORIDADE"; break;
    }

    return db->savePatient(idpessoa, getLogin(), curso, email, alunoUFPE, bolsistaPROAES, statusStr,
                           jaFezTriagem, temSolicitacao, emFila);
}

bool Paciente::updateInDatabase() {
    // Atualizar dados da pessoa
    int idpessoa = db->savePerson(getId(), getNome(), getIdade(), getCpf(), getTelefone(), getEndereco());
    if (idpessoa == -1) {
        return false;
    }

    setId(idpessoa);

    string statusStr;
    switch(status) {
        case VALIDADO: statusStr = "VALIDADO"; break;
        case NAO_VALIDADO: statusStr = "NAO_VALIDADO"; break;
        case PERDE_PRIORIDADE: statusStr = "PERDE_PRIORIDADE"; break;
    }

    return db->savePatient(idpessoa, getLogin(), curso, email, alunoUFPE, bolsistaPROAES, statusStr,
                           jaFezTriagem, temSolicitacao, emFila);
}

vector<Paciente*> Paciente::loadFromDatabase(Database* db) {
    vector<Paciente*> pacientes;

    auto persons = db->getAllPersons();
    auto patientData = db->getAllPatients();
    auto users = db->getAllUsers();

    // Criar um mapa de person_id para dados de paciente para facilitar a busca
    map<int, vector<string>> patientMap;
    for (const auto& p : patientData) {
        if (p.size() >= 7) {
            patientMap[stoi(p[0])] = p; // person_id -> dados do paciente
        }
    }

    // Criar um mapa de login para dados de usuário
    map<string, vector<string>> userMap;
    for (const auto& u : users) {
        if (u.size() >= 2) {
            userMap[u[0]] = u; // login -> dados do usuário
        }
    }

    for (const auto& person : persons) {
        if (person.size() < 6) continue;

        int idpessoa = stoi(person[0]);
        auto it = patientMap.find(idpessoa);
        if (it == patientMap.end()) continue; // Não é um paciente

        const auto& patient = it->second;
        string nome = person[1];
        int idade = stoi(person[2]);
        string cpf = person[3];
        string telefone = person[4];
        string endereco = person[5];

        string curso;
        string email;
        bool alunoUFPE = false;
        bool bolsistaPROAES = false;
        string statusStr;
        bool jaFezTriagem = false;
        bool temSolicitacao = false;
        bool emFila = false;
        string login = "paciente_" + to_string(idpessoa);

        if (patient.size() >= 9) {
            // Novo formato de linha de paciente com login armazenado
            login = patient[1];
            curso = patient[2];
            email = patient[3];
            alunoUFPE = stoi(patient[4]) != 0;
            bolsistaPROAES = stoi(patient[5]) != 0;
            statusStr = patient[6];
            jaFezTriagem = patient.size() > 7 ? stoi(patient[7]) != 0 : false;
            temSolicitacao = patient.size() > 8 ? stoi(patient[8]) != 0 : false;
            emFila = patient.size() > 9 ? stoi(patient[9]) != 0 : false;
        } else {
            // Formato antigo de linha de paciente, manter compatibilidade com versões anteriores
            curso = patient[1];
            email = patient[2];
            alunoUFPE = stoi(patient[3]) != 0;
            bolsistaPROAES = stoi(patient[4]) != 0;
            statusStr = patient[5];
            jaFezTriagem = patient.size() > 6 ? stoi(patient[6]) != 0 : false;
            temSolicitacao = patient.size() > 7 ? stoi(patient[7]) != 0 : false;
            emFila = patient.size() > 8 ? stoi(patient[8]) != 0 : false;
        }

        StatusPaciente status;
        if (statusStr == "VALIDADO") status = VALIDADO;
        else if (statusStr == "PERDE_PRIORIDADE") status = PERDE_PRIORIDADE;
        else status = NAO_VALIDADO;

        // Encontrar senha na tabela de usuários
        string senha = "password"; // Senha padrão
        auto userIt = userMap.find(login);
        if (userIt != userMap.end()) {
            const auto& userData = userIt->second;
            if (userData.size() >= 2) {
                senha = userData[1];
            }
        }

        Paciente* p = new Paciente(db, idpessoa, nome, idade, cpf, telefone, endereco,
                                  curso, email, alunoUFPE, bolsistaPROAES,
                                  login, senha, status, jaFezTriagem, temSolicitacao, emFila);

        // Carregar histórico
        auto history = db->getPatientHistory(idpessoa);
        for (const auto& h : history) {
            p->adicionarHistorico(h);
        }

        pacientes.push_back(p);
    }

    return pacientes;
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

