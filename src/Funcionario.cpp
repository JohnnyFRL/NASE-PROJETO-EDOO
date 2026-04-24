#include "Funcionario.hpp"
#include "Paciente.hpp"
#include <iostream>
#include <vector>

void realizarTriagem(vector<Usuario*>& usuarios, FilaPrioridade& fila);
void cadastrarPacienteSistema(vector<Usuario*>& usuarios);

using namespace std;

Funcionario::Funcionario(string login, string senha)
    : Usuario(login, senha) {}

void Funcionario::menu(vector<Usuario*>& usuarios, FilaPrioridade& fila){
    int opcao;

    do{
        cout << "\n--- Portal do Atendente ---" << endl;
        cout << "1. Cadastrar aluno" << endl;
        cout << "2. Ver historico" << endl;
        cout << "3. Realizar triagem" << endl;
        cout << "4. Chamar proximo paciente" << endl;
        cout << "5. Mostrar status das filas" << endl;
        cout << "6. Mostrar ordem das filas" << endl;
        cout << "7. Ver solicitacoes" << endl;
        cout << "0. Voltar" << endl;
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
                p->setEmFila(false);
                p->finalizarAtendimento(); // para atualizar paciente e pode pedir uma nova consulta depois.
            } else {
                cout << "Nenhum paciente na fila.\n";
            }
            break;
        }

        case 5:
            fila.mostrarFilas();
            break;

        case 6:
            fila.ordemFila();
            break;

        case 7: {
            bool encontrou = false;

            for(auto u : usuarios){
                Paciente* p = dynamic_cast<Paciente*>(u);

                if(p && p->possuiSolicitacao()){
                    encontrou = true;

                    int opc;
                    cout << "\nPaciente: " << p->getNome() << endl;
                    cout << "Sintomas: " << p->getDescricaoSolicitacao() << endl;

                    cout << "\n1. Marcar consulta\n";
                    cout << "2. Negar solicitacao\n";
                    cout << "3. Pular\n";
                    cout << "0. Voltar\n";
                    cout << "Escolha: ";
                    cin >> opc;

                    if(opc == 1){
                        string tipo;
                        int prioridade;

                        cin.ignore();

                        cout << "Tipo de atendimento: ";
                        getline(cin, tipo);

                        cout << "Prioridade (1-3): ";
                        cin >> prioridade;

                        while(prioridade < 1 || prioridade > 3){
                            cout << "Valor invalido! Digite 1, 2 ou 3: ";
                            cin >> prioridade;
                        }

                        Triagem* t = new Triagem(
                            p->getDescricaoSolicitacao(),
                            tipo,
                            prioridade
                        );

                        p->setTriagem(t);
                        fila.adicionarPaciente(p);

                        p->adicionarHistorico(
                            "Solicitacao: " + p->getDescricaoSolicitacao() + " -> ACEITA"
                        );

                        p->adicionarHistorico(
                            "Triagem: " + tipo +
                            " | Prioridade: " + to_string(prioridade)
                        );

                        p->limparSolicitacao();

                        cout << "Paciente adicionado a fila!\n";

                    } else if(opc == 2){

                        p->adicionarHistorico(
                            "Solicitacao: " + p->getDescricaoSolicitacao() + " -> NEGADA"
                        );

                        p->limparSolicitacao();

                        cout << "Solicitacao negada.\n";

                    } else if(opc == 0){
                        break;
                    }
                }
            }

            if(!encontrou){
                cout << "\nNenhuma solicitacao pendente.\n";
            }

            break;
        }

        case 0:
            cout << "Voltando ao menu...\n";
            break;

        default:
            cout << "Opcao invalida!\n";
        }

    } while(opcao != 0);
}