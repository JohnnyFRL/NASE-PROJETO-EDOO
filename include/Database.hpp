#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

class Database {
private:
    string dbPath;
    map<string, string> userTable;
    vector<vector<string>> personTable;
    vector<vector<string>> patientTable;
    vector<vector<string>> triageTable;
    vector<vector<string>> historyTable;

    void loadFromFile();
    void saveToFile();
    int nextId(const string& table);

public:
    Database(const string& path = "sistema_medico.db");
    ~Database();

    bool initialize();
    bool isConnected() const { return true; }

    // Operações de Usuário
    bool saveUser(const string& login, const string& senha, const string& tipo);
    vector<vector<string>> getAllUsers();
    bool userExists(const string& login);
    bool authenticateUser(const string& login, const string& senha);

    // Operações de Pessoa
    int savePerson(int id, const string& nome, int idade, const string& cpf,
                   const string& telefone, const string& endereco);
    vector<vector<string>> getAllPersons();

    // Operações de Paciente
    bool savePatient(int idpessoa, const string& login, const string& curso, const string& email,
                    bool alunoUFPE, bool bolsistaPROAES, const string& status,
                    bool jaFezTriagem, bool temSolicitacao, bool emFila);
    vector<vector<string>> getAllPatients();
    bool updatePatientStatus(int idpessoa, const string& status);
    bool addToHistory(int idpessoa, const string& registro);

    // Operações de Triagem
    int saveTriage(const string& sintomas, const string& tipoAtendimento, int prioridade);
    vector<vector<string>> getAllTriages();

    // Operações de Histórico
    bool saveHistory(int idpessoa, const string& registro);
    vector<string> getPatientHistory(int idpessoa);

    // Operações de Fila
    bool saveQueueState(const string& queueData);
    string loadQueueState();
};

#endif