#include <iostream>
#include <vector>
#include <string>
#include "Pessoa.hpp"
#include "Paciente.hpp"

using namespace std;
void exibirMenuFuncionario();
void cadastrarPaciente(vector<Paciente>& lista);

int main(){
    vector<Paciente> listaPacientes;
    int opcao;

    do{
        exibirMenuFuncionario();
        cin >> opcao;
        cin.ignore();

        if (opcao == 1){
            cadastrarPaciente(listaPacientes);

        } else if (opcao == 2){
            if (listaPacientes.empty()){
                cout << "\nNenhum paciente cadastrado." << endl;
            } else {
                cout << "\n--- Lista de Pacientes ---" << endl;
                for (size_t i = 0; i < listaPacientes.size(); i++) {
                    cout << i + 1 << ". " << listaPacientes[i].getPaciente() << endl;
                    cout << "   Status: " << listaPacientes[i].getStatus() << endl;
                    cout << "---------------------------" << endl;
                }
            }
        }
    }while (opcao != 0);

    cout << "Saindo do sistema..." << endl;
    return 0;
}

void exibirMenuFuncionario(){
    cout << "\n--- Portal do Atendente ---" << endl;
    cout << "1. Cadastrar aluno" << endl;
    cout << "2. ver historico" << endl;
    cout << "0. Sair" << endl;
    cout << "Escolha uma opcao: ";
}

void cadastrarPaciente(vector<Paciente>& lista){
    string nome, cpf, telefone, endereco, historico, curso, email;
    int idade;
    char souA, souB;
    bool alunoUFPE, bolsistaPROAES;

    cout << "\n--- Cadastro de Paciente ---" << endl;
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

    Paciente novoPaciente(nome, idade, cpf, telefone, endereco, historico, curso, email, alunoUFPE, bolsistaPROAES);
    novoPaciente.validar();
    lista.push_back(novoPaciente);
    
    cout << "\n[Sucesso] Paciente cadastrado e validado!" << endl;
}