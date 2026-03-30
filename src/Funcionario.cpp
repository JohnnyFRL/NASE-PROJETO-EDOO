#include "Funcionario.hpp"
#include "Paciente.hpp"
#include <iostream>
#include <vector>

void cadastrarPacienteSistema(std::vector<Usuario*>& usuarios);

using namespace std;

Funcionario::Funcionario(string login, string senha)
    : Usuario(login, senha){}

void Funcionario::menu(vector<Usuario*>& usuarios){
    int opcao;
    do{
        cout << "\n--- Portal do Atendente ---" << endl;
        cout << "1. Cadastrar aluno" << endl;
        cout << "2. ver historico" << endl;
        cout << "0. voltar" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;

         switch(opcao){
            case 1:
                cadastrarPacienteSistema(usuarios); 
                 break;
            case 2:
                for(auto u : usuarios){
        cout << "Login: " << u->getLogin() << endl;
    }
    break;
            case 0:
                cout << "Voltando ao menu...\n";
                break;
            default:
                cout << "Opcao invalida!\n";
        }

    }while(opcao != 0);
}