#ifndef PACIENTE_HPP
#define PACIENTE_HPP

#include "Pessoa.hpp"
#include <vector>
#include "Usuario.hpp"

#include <string>
using namespace std;

class Paciente : public Pessoa, public Usuario{

private:
    string historicoMedico;
    string curso;
    string email;
    bool alunoUFPE;
    bool bolsistaPROAES;
    string status; // "Validado", "Não Validado", ou " apto apenas Acolhimento"

public:
    Paciente(string nome, int idade, string cpf, string telefone, string endereco, string historicoMedico, string curso, string email, 
        bool alunoUFPE, bool bolsistaPROAES, string login, string senha);
    string getPaciente();
    void validar();
    string getStatus(); // status do paciente como valido ou não valido para triagem
        void menu(vector<Usuario*>& usuarios) override;

};
#endif