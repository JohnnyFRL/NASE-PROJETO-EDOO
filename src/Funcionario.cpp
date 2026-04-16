#include "Funcionario.hpp"
#include "Paciente.hpp"
#include <iostream>
#include <vector>

void realizarTriagem(std::vector<Usuario*>& usuarios, FilaPrioridade& fila);
void cadastrarPacienteSistema(std::vector<Usuario*>& usuarios);


using namespace std;

Funcionario::Funcionario(string login, string senha)
    : Usuario(login, senha){}

void Funcionario::menu(vector<Usuario*>& usuarios, FilaPrioridade& fila){
    int opcao;
    do{
        cout << "\n--- Portal do Atendente ---" << endl;
        cout << "1. Cadastrar aluno" << endl;
        cout << "2. ver historico" << endl;
        cout << "3. realizar triagem" << endl;
        cout << "4. chamar proximo paciente" << endl;
        cout << "5. mostrar status das filas" << endl;
        cout << "6. mostrar ordem das filas" << endl;
        cout << "7. Ver solicitacoes\n";
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
                 realizarTriagem(usuarios, fila);
    break;
            case 4: {
                 Paciente* p = fila.chamarProximo();
               if(p != nullptr){
               cout << "Chamando paciente...\n";
    }
    break;
}

             case 5:
                fila.mostrarFilas();
                break;
            case 6:
                fila.ordemFila();
                break;
            case 7:
            for(auto u : usuarios){
            Paciente* p = dynamic_cast<Paciente*>(u);
            if(p && p->possuiSolicitacao()){
            cout << "\nPaciente: " << p->getNome() << endl;
            cout << "Sintomas: " << p->getDescricaoSolicitacao() << endl;
            string tipo;
            int prioridade;
            cout << "Tipo de atendimento: ";
            cin.ignore();
            getline(cin, tipo);
            cout << "Prioridade (1-3): ";
            cin >> prioridade;
            Triagem* t = new Triagem(
                p->getDescricaoSolicitacao(),
                tipo,
                prioridade
            );
            p->setTriagem(t);
            fila.adicionarPaciente(p);
            p->limparSolicitacao();
            cout << "Paciente adicionado a fila!\n";
        }
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