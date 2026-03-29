#include <iostream>
#include <vector>
#include <string>
#include "Pessoa.hpp"
#include "Paciente.hpp"
#include "Usuario.hpp"
#include "Funcionario.hpp"

using namespace std;
void exibirMenuFuncionario();
void cadastrarPacienteSistema(vector<Usuario*>& usuarios);
Usuario* loginSistema(vector<Usuario*>& usuarios);

int main(){
    vector<Usuario*> usuarios;
    usuarios.push_back(new Funcionario("admin", "123"));

    int opcao;
    do{
        cout << "\n1. Login\n2. Cadastre-se\n0. Sair\n";
        cin >> opcao;

        if(opcao == 2){
            cadastrarPacienteSistema(usuarios);
        }

        if(opcao == 1){
            Usuario* u = loginSistema(usuarios);

            if(u != nullptr){
                u->menu(usuarios);
            } else {
                cout << "Login invalido\n";
            }
        }
    }while(opcao != 0);
    return 0;
}

void exibirMenuFuncionario(){
    cout << "\n--- Portal do Atendente ---" << endl;
    cout << "1. Cadastrar aluno" << endl;
    cout << "2. ver historico" << endl;
    cout << "0. voltar" << endl;
    cout << "Escolha uma opcao: ";
}

void cadastrarPacienteSistema(vector<Usuario*>& usuarios){
    string nome, cpf, telefone, endereco, historico, curso, email;
    int idade;
    string login, senha;
    char souA, souB;
    bool alunoUFPE, bolsistaPROAES;
     cout << "\n--- CADASTRO ---" << endl;

    cout << "Login: ";
    cin >> login;

    cout << "Senha: ";
    cin >> senha;
    cin.ignore();

    cout << "Nome: "; getline(cin, nome);
    cout << "Idade: "; cin >> idade; cin.ignore();
    cout << "CPF: "; getline(cin, cpf);
    cout << "Telefone: "; getline(cin, telefone);
    cout << "Endereco: "; getline(cin, endereco);
    cout << "Historico Medico: "; getline(cin, historico);
    cout << "Curso: "; getline(cin, curso);
    cout << "Email: "; getline(cin, email);
    
    cout << "E aluno da UFPE? (s/n): "; cin >> souA;
    alunoUFPE = (souA == 's' || souA == 'S');

    cout << "E bolsista PROAES? (s/n): "; cin >> souB;
    bolsistaPROAES = (souB == 's' || souB == 'S');

     Paciente* novo = new Paciente(
        nome, idade, cpf, telefone, endereco,
        historico, curso, email,
        alunoUFPE, bolsistaPROAES,
        login, senha
    );
    novo ->validar();

    usuarios.push_back(novo);
    
    cout << "\n Paciente cadastrado" << endl;
}

Usuario* loginSistema(vector<Usuario*>& usuarios){
    string login, senha;

    cout << "Login: ";
    cin >> login;

    cout << "Senha: ";
    cin >> senha;

    for(auto u : usuarios){
        if(u->getLogin() == login && u->autenticar(senha)){
            return u;
        }
    }

    return nullptr;
}