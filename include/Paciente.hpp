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
    string curso;
    string email;
    bool alunoUFPE;
    bool bolsistaPROAES;
    StatusPaciente status; // "Validado", "Não Validado", ou " apto apenas Acolhimento"
    vector<string> historicoSolicitacoes; // aluno vai ter um historico de solicitacoes de consulta, para o funcionario poder avaliar se o paciente tem um padrao de solicitacao que pode indicar abuso do sistema
    string descricaoSolicitacao;
    Triagem* triagem; // Associação com a classe Triagem
    bool emFila;
    
public:
    Paciente(string nome, int idade, string cpf, string telefone, string endereco, string curso, string email, 
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
    string getDescricaoSolicitacao();
    bool possuiSolicitacao();
    void limparSolicitacao();
    void adicionarHistorico(string registro);
    vector<string> getHistoricoSolicitacoes();
    void finalizarAtendimento(); // correção de erro para atualizar quando o paciente sai da fila para pode pedir uma nova consulta depois, já que o sistema só permite uma solicitacao ativa por paciente
    void mostrarHistorico();
    bool jaFezTriagem; // para evitar que o paciente faça triagem mais de uma vez e fique com múltiplas triagens ativas, o que causava um bug na hora de finalizar o atendimento, já que o sistema só atualizava o status do paciente para "atendido" na última triagem feita, e as outras ficavam ativas, fazendo com que o paciente não pudesse pedir uma nova consulta depois de finalizar a primeira, já que o sistema achava que ele ainda tinha uma triagem ativa
    bool temSolicitacao; // correção de bug de duplicado na triagem
    bool estaNaFila();
    void setEmFila(bool valor);
};
#endif