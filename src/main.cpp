#include <iostream>
#include <vector>
#include <string>
#include "Pessoa.hpp"
#include "Paciente.hpp"
#include "Usuario.hpp"
#include "Funcionario.hpp"
#include "Triagem.hpp"
#include "FilaPrioridade.hpp"

#include <limits>

using namespace std;
void cadastrarPacienteSistema(vector<Usuario*>& usuarios);
Usuario* loginSistema(vector<Usuario*>& usuarios, FilaPrioridade& fila);
void realizarTriagem(vector<Usuario*>& usuarios, FilaPrioridade& fila);

int main(){
    FilaPrioridade fila;
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
    string sintomas,tipoAtendimento; // triagem
    string numeros = "";
    int idade;
    int prioridade; // triagem
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
     while(true){
        cout << "Idade: ";
        cin >> idade;
        if(cin.fail()){
            cout << "invalido! \n";
            cin.clear();
            cin.ignore(1000, '\n');
        } else if(idade <= 0){
            cout << "Idade invalida!\n";
        } else{
            cin.ignore();
            break;
        }
    }
    while(true){
    cout << "CPF: ";
    getline(cin, cpf);
    numeros = "";
    bool valido = true;
    for(char c : cpf){
        if(c >= '0' && c <= '9'){
            numeros += c;
        } 
        else if(c == '.' || c == '-'){
            continue;
        } 
        else {
            valido = false;
            break;
        }
    }
    if(!valido){
        cout << "CPF invalido! Use apenas numeros, pontos ou traço.\n";
        continue;
    }
    if(numeros.size() != 11){
        cout << "CPF deve ter 11 digitos!\n";
        continue;
    }
    break;
}
    cout << "Telefone: "; getline(cin, telefone);
    cout << "Endereco: "; getline(cin, endereco);
    cout << "Historico Medico: "; getline(cin, historico);
    cout << "Curso: "; getline(cin, curso);
    cout << "Email: "; getline(cin, email);
    
    cout << "E aluno da UFPE? (s/n): "; cin >> souA;
    alunoUFPE = (souA == 's' || souA == 'S');
    if(alunoUFPE != true){
    cout << "\n Apenas alunos da UFPE podem se cadastrar no sistema. \n";
    return;
}
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

Usuario* loginSistema(vector<Usuario*>& usuarios, FilaPrioridade& fila){
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

void realizarTriagem(vector<Usuario*>& usuarios, FilaPrioridade& fila){
    
    string login;
    cout << "\nDigite o login do paciente: ";
    cin >> login;

    for(auto u : usuarios){
        Paciente* p = dynamic_cast<Paciente*>(u);
        if(p && p->getLogin() == login){
            string sintomas, tipo;
            int prioridade;

            cout << "\n--- TRIAGEM ---\n";

            cout << "escreva os sintomas do paciente: ";
            getline(cin, sintomas);

            cout << "Tipo de atendimento: ";
            getline(cin, tipo);

            cout << "Prioridade (1-alta | 2-media | 3-baixa): ";
            cin >> prioridade;

            while(prioridade < 1 || prioridade > 3){
                cout << "Valor invalido! Digite 1, 2 ou 3: ";
                cin >> prioridade;
            }

            Triagem* t = new Triagem(sintomas, tipo, prioridade);
            p->setTriagem(t);
            fila.adicionarPaciente(p);

            cout << "\n Triagem realizada com sucesso!\n";
            return;
        }
    }

    cout << "\n Paciente nao encontrado.\n";
}