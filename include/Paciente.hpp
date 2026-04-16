#ifndef PACIENTE_HPP
#define PACIENTE_HPP

#include "Pessoa.hpp"
#include <vector>
#include "Usuario.hpp"
#include "Triagem.hpp"


#include <string>
using namespace std;


enum StatusPaciente {
    NAO_VALIDADO,
    VALIDADO,
    PERDE_PRIORIDADE,
};


class FilaPrioridade; // evitar erro de declaracao circular, já que FilaPrioridade inclui Paciente e Paciente inclui FilaPrioridade
class Paciente : public Pessoa, public Usuario{

private:
    string historicoMedico;
    string curso;
    string email;
    bool alunoUFPE;
    bool bolsistaPROAES;
    StatusPaciente status; // "Validado", "Não Validado", ou " apto apenas Acolhimento"
    vector<string> historicoSolicitacoes; // aluno vai ter um historico de solicitacoes de consulta, para o funcionario poder avaliar se o paciente tem um padrao de solicitacao que pode indicar abuso do sistema

    Triagem* triagem; // Associação com a classe Triagem

public:
    Paciente(string nome, int idade, string cpf, string telefone, string endereco, string historicoMedico, string curso, string email, 
        bool alunoUFPE, bool bolsistaPROAES, string login, string senha);
    string getPaciente();
    void validar();
    string getStatus(); // status do paciente como valido ou não valido para triagem
    
    Triagem* getTriagem(); 
    void setTriagem(Triagem* triagem);

    void menu(vector<Usuario*>& usuarios, FilaPrioridade& fila) override;
    bool isAlunoUFPE();
    bool isBolsistaPROAES();
    StatusPaciente getStatusEnum();
    void solicitarConsulta();
    string descricaoSolicitacao;
    bool temSolicitacao;
    string getDescricaoSolicitacao();
    bool possuiSolicitacao();
    void limparSolicitacao();
    void adicionarHistorico(string registro);
    vector<string> getHistoricoSolicitacoes();
    void finalizarAtendimento(); // correção de erro para atualizar quando o paciente sai da fila para pode pedir uma nova consulta depois, já que o sistema só permite uma solicitacao ativa por paciente
    void mostrarHistorico();
};
#endif