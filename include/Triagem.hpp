#ifndef TRIAGEM_HPP
#define TRIAGEM_HPP 
#include <string>

using namespace std;

class Triagem {
private:
    string sintomas;
    string tipoAtendimento;
    int prioridade;
public:
    Triagem(string sintomas, string tipoAtendimento, int prioridade);
    string getSintomas();
    string getTipoAtendimento();
    string getResumo();
    int getPrioridade();
};

#endif