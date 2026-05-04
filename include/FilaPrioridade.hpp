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
    // adaptcao para tamanho interface do qt
    int totalAlta()const{
         return (int)filaAlta.size();
        }
    int totalMedia()const{ 
        return (int)filaMedia.size(); 
    }
    int totalBaixa()const{ 
        return (int)filaBaixa.size(); 
    }
    int totalMuitoBaixa()const
    { 
        return (int)filaMuitoBaixa.size();
     }
    int total()const
    { 
        return totalAlta() + totalMedia() + totalBaixa() + totalMuitoBaixa();
     }
     
         // Retorna cópia ordenada de todos os pacientes na fila (rank completo)
     vector<pair<Paciente*, int>> getRankFila()const{
        vector<pair<Paciente*, int>> rank;
        queue<Paciente*> tmp;

        tmp = filaAlta;
        while (!tmp.empty()){ 
            rank.push_back({tmp.front(), 1}); 
            tmp.pop(); 
        }
        tmp = filaMedia;
        while(!tmp.empty()){ 
            rank.push_back({tmp.front(), 2}); tmp.pop();
        }
        tmp = filaBaixa;
        while (!tmp.empty()){ 
            rank.push_back({tmp.front(), 3}); tmp.pop();
        }
        tmp = filaMuitoBaixa;
        while (!tmp.empty()){ 
            rank.push_back({tmp.front(), 4}); tmp.pop();
        }

        return rank;
    }
};

#endif