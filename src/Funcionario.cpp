#include "Funcionario.hpp"
#include "Paciente.hpp"
#include <iostream>
#include <vector>

void realizarTriagem(std::vector<Usuario*>& usuarios);
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
        cout << "3. realizar triagem" << endl;
        cout << "0. voltar" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;

         switch(opcao){
            case 1:
                cadastrarPacienteSistema(usuarios); 
                 break;
            case 2:
                for(auto u : usuarios){
         Paciente* p = dynamic_cast<Paciente*>(u);
    if(p){
        cout << p->getPaciente() << endl;
    }
    }
    break;
            case 3:
                 realizarTriagem(usuarios);
    break;
            case 0:
                cout << "Voltando ao menu...\n";
                break;
            default:
                cout << "Opcao invalida!\n";
        }

    }while(opcao != 0);
}