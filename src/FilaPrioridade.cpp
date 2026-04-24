#include "FilaPrioridade.hpp"
#include <iostream>

using namespace std;

void FilaPrioridade::adicionarPaciente(Paciente* p) {
    if(p->estaNaFila()){
       cout << "Paciente ja esta na fila!\n";
       return;
    }

    if (p->getTriagem() == nullptr) {
        cout << "Paciente sem triagem, nao pode ser adicionado a fila.\n";
        return;
    }
     if(p->getStatusEnum() == NAO_VALIDADO){
        cout << "Paciente nao validado nao entra na fila.\n";
        return;
    }
     if(p->possuiSolicitacao()){
    p->limparSolicitacao();
    }

    int prioridade = p->getTriagem()->getPrioridade();
    if(p->getStatusEnum() == PERDE_PRIORIDADE){
        prioridade = 4;
    }

    if(prioridade == 1){
        filaAlta.push(p);
    }
    else if(prioridade == 2){
        filaMedia.push(p);
    }else if(prioridade == 3){
    filaBaixa.push(p);
    }else{
        filaMuitoBaixa.push(p); // não bolsistas vão pra prioridade 4, que é a mais baixa
    }
    p->setEmFila(true);
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
    }else if (!filaMuitoBaixa.empty()) {
    Paciente* p = filaMuitoBaixa.front();
    filaMuitoBaixa.pop();
    return p;
    }else{
        cout << "Fila vazia.\n";
        return nullptr;
    }
}

void FilaPrioridade::mostrarFilas(){
    cout << "\n--- STATUS DAS FILAS ---\n";
    cout << "Prioridade 1: " << filaAlta.size() << " pacientes\n";
    cout << "Prioridade 2: " << filaMedia.size() << " pacientes\n";
    cout << "Prioridade 3: " << filaBaixa.size() << " pacientes\n";
    cout << "Prioridade 4: " << filaMuitoBaixa.size() << " pacientes\n";
}

void FilaPrioridade::ordemFila(){
    cout << "\n--- ORDEM DE CHAMADA ---\n";
    int posicao = 1;

     queue<Paciente*> alta = filaAlta;
     queue<Paciente*> media = filaMedia;
     queue<Paciente*> baixa = filaBaixa;
     queue<Paciente*> muitoBaixa = filaMuitoBaixa;
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
    while(!muitoBaixa.empty()){
    Paciente* p = muitoBaixa.front();
    cout << posicao++ << "º " << p->getNome() << " (Muito Baixa)\n";
    muitoBaixa.pop();
    }

    if(posicao == 1){
        cout << "Fila vazia.\n";
    }
}