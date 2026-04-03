#include "Triagem.hpp"

Triagem::Triagem(string sintomas, string tipoAtendimento, int prioridade){
    this->sintomas = sintomas;
    this->prioridade = prioridade;
    this->tipoAtendimento = tipoAtendimento;
}
string Triagem::getSintomas(){
    return sintomas;
}
string Triagem::getResumo(){
    return "Sintomas: " + sintomas + 
           " | Prioridade: " + to_string(prioridade) +
           " | Tipo: " + tipoAtendimento;
}
string Triagem::getTipoAtendimento(){
    return tipoAtendimento;
}

int Triagem::getPrioridade(){
    return prioridade;
}