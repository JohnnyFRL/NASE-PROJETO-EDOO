#include "../include/Pessoa.hpp"

int main() {
    Pessoa p("vitor", 25);
    cout << "Nome: " << p.getNome() << endl;
    cout << "Idade: " << p.getIdade() << endl;


    return 0;
}