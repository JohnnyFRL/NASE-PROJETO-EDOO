#include "Database.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ctime>

using namespace std;

//funções para impedir o salvamento incorreto na base de dados
static string escapeField(const string& field) {
    string escaped;
    for (char ch : field) {
        if (ch == '\\' || ch == ',') {
            escaped.push_back('\\');
        }
        escaped.push_back(ch);
    }
    return escaped;
}

static vector<string> splitEscaped(const string& line) {
    vector<string> fields;
    string current;
    bool escaping = false;

    for (char ch : line) {
        if (escaping) {
            current.push_back(ch);
            escaping = false;
            continue;
        }
        if (ch == '\\') {
            escaping = true;
            continue;
        }
        if (ch == ',') {
            fields.push_back(current);
            current.clear();
            continue;
        }
        current.push_back(ch);
    }
    fields.push_back(current);
    return fields;
}

Database::Database(const string& path) : dbPath(path) {}

Database::~Database() {
    saveToFile();
}

void Database::loadFromFile() {
    ifstream file(dbPath);
    if (!file.is_open()) return;

    string line;
    string currentTable;

    while (getline(file, line)) { //construção das tabelas da base de dados
        if (line.empty()) continue;

        if (line == "[USUARIOS]" || line == "[USERS]") {
            currentTable = "usuarios";
            continue;
        } else if (line == "[PESSOAS]" || line == "[PERSONS]") {
            currentTable = "pessoas";
            continue;
        } else if (line == "[PACIENTES]" || line == "[PATIENTS]") {
            currentTable = "pacientes";
            continue;
        } else if (line == "[TRIAGENS]" || line == "[TRIAGES]") {
            currentTable = "triagens";
            continue;
        } else if (line == "[HISTORICO]" || line == "[HISTORY]") {
            currentTable = "historico";
            continue;
        } else if (line == "[ATENDIMENTO]" || line == "[ATTENDANCE]") {
            currentTable = "atendimento";
            continue;
        }

        //lógica para mapear e popular corrretamente as tabelas
        if (currentTable == "usuarios") {
            auto row = splitEscaped(line);
            if (row.size() >= 3) {
                userTable[row[0]] = row[1] + "," + row[2];
            } else if (row.size() == 2) {
                userTable[row[0]] = row[1] + ",";
            }
        } else if (currentTable == "pessoas") {
            auto row = splitEscaped(line);
            if (row.size() >= 6) personTable.push_back(row);
        } else if (currentTable == "pacientes") {
            auto row = splitEscaped(line);
            if (row.size() >= 6) patientTable.push_back(row);
        } else if (currentTable == "triagens") {
            auto row = splitEscaped(line);
            if (row.size() >= 4) triageTable.push_back(row);
        } else if (currentTable == "historico") {
            auto row = splitEscaped(line);
            if (row.size() >= 3) historyTable.push_back(row);
        } else if (currentTable == "atendimento") {
            auto row = splitEscaped(line);
            if (row.size() >= 3) {
                atendimentoTable[row[0]] = {row[0], row[1], row[2]}; // login, atendimentos_hoje, atendimentos_realizados
            }
        }
    }
}

void Database::saveToFile() {
    ofstream file(dbPath);
    if (!file.is_open()) return;

    // Salvar Usuários
    file << "[USUARIOS]\n";
    for (const auto& pair : userTable) {
        auto row = splitEscaped(pair.second);
        string senha = row.size() > 0 ? row[0] : "";
        string tipo = row.size() > 1 ? row[1] : "";
        file << escapeField(pair.first) << "," << escapeField(senha) << "," << escapeField(tipo) << "\n";
    }

    // Salvar Pessoas
    file << "[PESSOAS]\n";
    for (const auto& row : personTable) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << escapeField(row[i]);
            if (i < row.size() - 1) file << ",";
        }
        file << "\n";
    }

    // Salvar Pacientes
    file << "[PACIENTES]\n";
    for (const auto& row : patientTable) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << escapeField(row[i]);
            if (i < row.size() - 1) file << ",";
        }
        file << "\n";
    }

    // Salvar triagens
    file << "[TRIAGENS]\n";
    for (const auto& row : triageTable) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << escapeField(row[i]);
            if (i < row.size() - 1) file << ",";
        }
        file << "\n";
    }

    // Salvar Histórico
    file << "[HISTORICO]\n";
    for (const auto& row : historyTable) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << escapeField(row[i]);
            if (i < row.size() - 1) file << ",";
        }
        file << "\n";
    }

    // Salvar Atendimento
    file << "[ATENDIMENTO]\n";
    for (const auto& pair : atendimentoTable) {
        file << escapeField(pair.first) << "," << escapeField(pair.second[1]) << "," << escapeField(pair.second[2]) << "\n";
    }
}

int Database::nextId(const string& table) {
    int maxId = 0;
    if (table == "persons") {
        for (const auto& row : personTable) {
            if (!row.empty()) {
                int id = stoi(row[0]);
                if (id > maxId) maxId = id;
            }
        }
    } else if (table == "triages") {
        for (const auto& row : triageTable) {
            if (!row.empty()) {
                int id = stoi(row[0]);
                if (id > maxId) maxId = id;
            }
        }
    }
    return maxId + 1;
}

bool Database::initialize() {
    loadFromFile();
    return true;
}

// Operações de usuários
bool Database::saveUser(const string& login, const string& senha, const string& tipo) {
    userTable[login] = senha + "," + tipo;
    saveToFile();
    return true;
}

vector<vector<string>> Database::getAllUsers() {
    vector<vector<string>> result;
    for (const auto& pair : userTable) {
        vector<string> row;
        row.push_back(pair.first); // login
        auto fields = splitEscaped(pair.second);
        string senha = fields.size() > 0 ? fields[0] : "";
        string tipo = fields.size() > 1 ? fields[1] : "";
        row.push_back(senha);
        row.push_back(tipo);
        result.push_back(row);
    }
    return result;
}

bool Database::userExists(const string& login) {
    return userTable.find(login) != userTable.end();
}

bool Database::authenticateUser(const string& login, const string& senha) {
    auto it = userTable.find(login);
    if (it == userTable.end()) return false;

    auto fields = splitEscaped(it->second);
    string storedSenha = fields.size() > 0 ? fields[0] : "";
    return storedSenha == senha;
}

// Operações de Pessoas
int Database::savePerson(int id, const string& nome, int idade, const string& cpf,
                         const string& telefone, const string& endereco) {
    vector<string> row;
    if (id == -1) {
        // Se o CPF já existe, usar o mesmo ID
        for (const auto& existing : personTable) {
            if (existing.size() >= 4 && existing[3] == cpf) {
                id = stoi(existing[0]);
                break;
            }
        }
        if (id == -1) {
            id = nextId("persons");
        }
    }

    row.push_back(to_string(id));
    row.push_back(nome);
    row.push_back(to_string(idade));
    row.push_back(cpf);
    row.push_back(telefone);
    row.push_back(endereco);

    // Remover a pessoa com mesmo ID (sobrescrever pessoa)
    personTable.erase(
        remove_if(personTable.begin(), personTable.end(),
            [id](const vector<string>& r) {
                return !r.empty() && stoi(r[0]) == id;
            }),
        personTable.end()
    );

    personTable.push_back(row);
    saveToFile();
    return id;
}

vector<vector<string>> Database::getAllPersons() {
    return personTable;
}

// Operações de paciente
bool Database::savePatient(int idpessoa, const string& login, const string& curso, const string& email,
                          bool alunoUFPE, bool bolsistaPROAES, const string& status,
                          bool jaFezTriagem, bool temSolicitacao, bool emFila) {
    vector<string> row;
    row.push_back(to_string(idpessoa));
    row.push_back(login);
    row.push_back(curso);
    row.push_back(email);
    row.push_back(alunoUFPE ? "1" : "0");
    row.push_back(bolsistaPROAES ? "1" : "0");
    row.push_back(status);
    row.push_back(jaFezTriagem ? "1" : "0");
    row.push_back(temSolicitacao ? "1" : "0");
    row.push_back(emFila ? "1" : "0");

    // Remover paciente que tem o mesmo id do cadastrado (sobrescrever)
    patientTable.erase(
        remove_if(patientTable.begin(), patientTable.end(),
            [idpessoa](const vector<string>& r) {
                return !r.empty() && stoi(r[0]) == idpessoa;
            }),
        patientTable.end()
    );

    patientTable.push_back(row);
    saveToFile();
    return true;
}

vector<vector<string>> Database::getAllPatients() {
    return patientTable;
}

bool Database::updatePatientStatus(int idpessoa, const string& status) {
    for (auto& row : patientTable) {
        if (!row.empty() && stoi(row[0]) == idpessoa) {
            if (row.size() > 6) {
                row[6] = status;
            }
            saveToFile();
            return true;
        }
    }
    return false;
}

bool Database::addToHistory(int idpessoa, const string& registro) {
    return saveHistory(idpessoa, registro);
}

// Operações de Triagem
int Database::saveTriage(const string& sintomas, const string& tipoAtendimento, int prioridade) {
    int id = nextId("triages");
    vector<string> row;
    row.push_back(to_string(id));
    row.push_back(sintomas);
    row.push_back(tipoAtendimento);
    row.push_back(to_string(prioridade));
    triageTable.push_back(row);
    saveToFile();
    return id;
}

vector<vector<string>> Database::getAllTriages() {
    return triageTable;
}

// Operações de Histórico
bool Database::saveHistory(int idpessoa, const string& registro) {
    time_t now = time(nullptr);
    vector<string> row;
    row.push_back(to_string(historyTable.size() + 1));
    row.push_back(to_string(idpessoa));
    row.push_back(registro);
    row.push_back(to_string(now));
    historyTable.push_back(row);
    saveToFile();
    return true;
}

vector<string> Database::getPatientHistory(int idpessoa) {
    vector<string> history;
    for (const auto& row : historyTable) {
        if (row.size() >= 3 && stoi(row[1]) == idpessoa) {
            history.push_back(row[2]); // registro
        }
    }
    return history;
}

// Operações de fila (salva como comentário no arquivo)
bool Database::saveQueueState(const string& queueData) {
    return true;
}

string Database::loadQueueState() {
    return "";
}

// Operações de Atendimento
bool Database::saveAttendance(const string& login, int atendimentosHoje, int atendimentosRealizados) {
    atendimentoTable[login] = {login, to_string(atendimentosHoje), to_string(atendimentosRealizados)};
    saveToFile();
    return true;
}

bool Database::loadAttendance(const string& login, int& atendimentosHoje, int& atendimentosRealizados) {
    auto it = atendimentoTable.find(login);
    if (it != atendimentoTable.end()) {
        try {
            atendimentosHoje = stoi(it->second[1]);
            atendimentosRealizados = stoi(it->second[2]);
            return true;
        } catch (...) {
            return false;
        }
    }
    return false;
}