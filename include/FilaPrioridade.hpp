#ifndef FILAPRIORIDADE_HPP
#define FILAPRIORIDADE_HPP

#include "paciente.hpp"
#include <queue>

using namespace std;    

class FilaPrioridade {
private:
    queue<Paciente*> filaAlta;
    queue<Paciente*> filaMedia;
    queue<Paciente*> filaBaixa;
    queue<Paciente*> filaMuitoBaixa;

public:
 void adicionarPaciente(Paciente* p);
    Paciente* chamarProximo();
    bool vazia();
    void mostrarFilas();
    void ordemFila();
};

#endif