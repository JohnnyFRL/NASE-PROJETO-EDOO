#include <iostream>
#include <vector>
#include <string>
#include "Pessoa.hpp"
#include "Paciente.hpp"
#include "Usuario.hpp"
#include "Funcionario.hpp"
#include "Triagem.hpp"
#include "FilaPrioridade.hpp"
#include "Validacao.hpp"   

#include <limits>

using namespace std;
void cadastrarPacienteSistema(vector<Usuario*>& usuarios);
Usuario* loginSistema(vector<Usuario*>& usuarios, FilaPrioridade& fila);
void realizarTriagem(vector<Usuario*>& usuarios, FilaPrioridade& fila);

int main(){
    FilaPrioridade fila;
    vector<Usuario*> usuarios;
    usuarios.push_back(new Funcionario("admin", "Admin@123"));
    int opcao;
    
    do{
        cout << "\n1. Login\n2. Cadastre-se\n0. Sair\n";
        cin >> opcao;

        if(opcao == 2){
            cadastrarPacienteSistema(usuarios);
        }

        if(opcao == 1){
            Usuario* u = loginSistema(usuarios, fila);

            if(u != nullptr){
                u->menu(usuarios, fila);
            } else {
                cout << "Login invalido\n";
            }
        }
    }while(opcao != 0);
    return 0;
}


void cadastrarPacienteSistema(vector<Usuario*>& usuarios){
    string nome, cpf, telefone, endereco, historico, curso, email;
    string numeros = "";
    int idade;
    string login, senha;
    string erro;
    char souA, souB;
    bool alunoUFPE, bolsistaPROAES;

    cout << "\n--- CADASTRO ---" << endl;

    // [T3] Login unico
    while(true){
        cout << "Login: ";
        cin >> login;
        if(Validacao::validarLoginUnico(login, usuarios, erro)){
            break;
        }
        cout << "[ERRO] " << erro << "\n";
    }

    // [T2] Senha forte
    while(true){
        cout << "Senha (min. 8 chars, maiuscula, minuscula, numero e simbolo): ";
        cin >> senha;
        if(Validacao::validarSenhaForte(senha, erro)){
            break;
        }
        cout << "[ERRO] " << erro << "\n";
    }
    cin.ignore();

    // [T1] Validacao de nome
    while(true){
        cout << "Nome completo: ";
        getline(cin, nome);
        if(Validacao::validarNome(nome, erro)){
            break;
        }
        cout << "[ERRO] " << erro << "\n";
    }

    // Validacao de idade (original)
    string idadeStr;

    while(true){
        cout << "Idade: ";
        getline(cin, idadeStr);

        if(idadeStr.empty()){
        cout << "[ERRO] Idade nao pode ser vazia!\n";
        continue;
    }
        bool soNumero = true;
        for(char c : idadeStr){
        if(c < '0' || c > '9'){
            soNumero = false;
            break;
        }
    }
        if(!soNumero){
        cout << "[ERRO] Digite apenas numeros!\n";
        continue;
    }
        idade = stoi(idadeStr);
        if(idade <= 0 || idade > 130){
            cout << "[ERRO] Idade invalida!\n";
            continue;
    }
    break;
}

    // Validacao de CPF (original)
    while(true){
        cout << "CPF: ";
        getline(cin, cpf);
        numeros = "";
        bool valido = true;
        for(int i = 0; i < (int)cpf.length(); i++){
            char c = cpf[i];
            if(c >= '0' && c <= '9'){
                numeros += c;
            } else if(c == '.' || c == '-'){
                continue;
            } else {
                valido = false;
                break;
            }
        }
        if(!valido){
            cout << "[ERRO] CPF invalido! Use apenas numeros, pontos ou traco.\n";
            continue;
        }
        if((int)numeros.length() != 11){
            cout << "[ERRO] CPF deve ter 11 digitos!\n";
            continue;
        }
        break;
    }

    // [T5] Validacao de telefone
    while(true){
        cout << "Telefone (ex: (81) 91234-5678): ";
        getline(cin, telefone);
        if(Validacao::validarTelefone(telefone, erro)){
            break;
        }
        cout << "[ERRO] " << erro << "\n";
    }

    cout << "Endereco: ";
    getline(cin, endereco);

    cout << "Historico Medico: ";
    getline(cin, historico);

    cout << "Curso: ";
    getline(cin, curso);

    // [T4] Validacao de e-mail
    while(true){
        cout << "Email (ex: aluno@ufpe.br): ";
        getline(cin, email);
        if(Validacao::validarEmail(email, erro)){
            break;
        }
        cout << "[ERRO] " << erro << "\n";
    }

    cout << "E aluno da UFPE? (s/n): ";
    cin >> souA;
    alunoUFPE = (souA == 's' || souA == 'S');

    if(!alunoUFPE){
        cout << "\nApenas alunos da UFPE podem se cadastrar no sistema.\n";
        return;
    }

    cout << "E bolsista PROAES? (s/n): ";
    cin >> souB;
    bolsistaPROAES = (souB == 's' || souB == 'S');

    Paciente* novo = new Paciente(
        nome, idade, cpf, telefone, endereco,
        historico, curso, email,
        alunoUFPE, bolsistaPROAES,
        login, senha
    );
    novo->validar();

    usuarios.push_back(novo);

    cout << "\nPaciente cadastrado com sucesso!" << endl;
}

Usuario* loginSistema(vector<Usuario*>& usuarios, FilaPrioridade& fila){
    string login, senha;

    cout << "Login: ";
    cin >> login;

    cout << "Senha: ";
    cin >> senha;

    for(int i = 0; i < (int)usuarios.size(); i++){
        if(usuarios[i]->getLogin() == login && usuarios[i]->autenticar(senha)){
            return usuarios[i];
        }
    }

    return nullptr;
}

void realizarTriagem(vector<Usuario*>& usuarios, FilaPrioridade& fila){
    string login;
    cout << "\nDigite o login do paciente: ";
    cin >> login;

    for(int i = 0; i < (int)usuarios.size(); i++){
        Paciente* p = dynamic_cast<Paciente*>(usuarios[i]);
        if(p && p->getLogin() == login){
            if(p->jaFezTriagem){
    cout << "Paciente ja passou pela triagem inicial.\n";
    return;
}
            string sintomas, tipo;
            int prioridade;

            cout << "\n--- TRIAGEM ---\n";
            cout << "escreva os sintomas do paciente: ";
            cin.ignore();

            getline(cin, sintomas);

            cout << "Prioridade (1-alta | 2-media | 3-baixa): ";
            cin >> prioridade;

            while(prioridade < 1 || prioridade > 3){
                cout << "Valor invalido! Digite 1, 2 ou 3: ";
                cin >> prioridade;
            }

            Triagem* t = new Triagem(sintomas, "Triagem Inicial", prioridade);
            p->setTriagem(t);
            fila.adicionarPaciente(p);
            p->adicionarHistorico(
           "Triagem inicial: Sintomas: " + sintomas +
           " | Tipo: " + tipo + 
           " | Prioridade: " + to_string(prioridade)
             );
            p->jaFezTriagem = true;
            p->temSolicitacao = false; //correcao do bug de paciente duplicado na triagem

            p->limparSolicitacao();
            cout << "\nTriagem realizada com sucesso!\n";
            return;
        }
    }

    cout << "\nPaciente nao encontrado.\n";
}
