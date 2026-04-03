#include "FilaPrioridade.hpp"
#include <iostream>

using namespace std;

void FilaPrioridade::adicionarPaciente(Paciente* p) {
    if (p->getTriagem() == nullptr) {
        cout << "Paciente sem triagem, nao pode ser adicionado a fila.\n";
        return;
    }

     int prioridade = p->getTriagem()->getPrioridade();
    if (prioridade == 1) {
        filaAlta.push(p);
        cout << "Paciente adicionado a fila de alta prioridade.\n";
    } else if (prioridade == 2) {
        filaMedia.push(p);
        cout << "Paciente adicionado a fila de media prioridade.\n";
    } else {
        filaBaixa.push(p);
        cout << "Paciente adicionado a fila de baixa prioridade.\n";
    }
    
}

 Paciente * FilaPrioridade::chamarProximo() {
    if (!filaAlta.empty()) {
        Paciente* p = filaAlta.front();
        filaAlta.pop();
        return p;
    } else if (!filaMedia.empty()) {
        Paciente* p = filaMedia.front();
        filaMedia.pop();
        return p;
    } else if (!filaBaixa.empty()) {
        Paciente* p = filaBaixa.front();
        filaBaixa.pop();
        return p;
    } else {
        cout << "Fila vazia.\n";
        return nullptr;
    }
}

void FilaPrioridade::mostrarFilas(){
    cout << "\n--- STATUS DAS FILAS ---\n";
    cout << "Prioridade 1: " << filaAlta.size() << " pacientes\n";
    cout << "Prioridade 2: " << filaMedia.size() << " pacientes\n";
    cout << "Prioridade 3: " << filaBaixa.size() << " pacientes\n";
}

void FilaPrioridade::ordemFila(){
    cout << "\n--- ORDEM DE CHAMADA ---\n";
    int posicao = 1;

     queue<Paciente*> alta = filaAlta;
     queue<Paciente*> media = filaMedia;
     queue<Paciente*> baixa = filaBaixa;

    while(!alta.empty()){
        Paciente* p = alta.front();
        cout << posicao++ << "º " << p->getNome() << " (Alta)\n";
        alta.pop();
    }
    while(!media.empty()){
        Paciente* p = media.front();
        cout << posicao++ << "º " << p->getNome() << " (Media)\n";
        media.pop();
    }

    while(!baixa.empty()){
        Paciente* p = baixa.front();
        cout << posicao++ << "º " << p->getNome() << " (Baixa)\n";
        baixa.pop();
    }

    if(posicao == 1){
        cout << "Fila vazia.\n";
    }
}