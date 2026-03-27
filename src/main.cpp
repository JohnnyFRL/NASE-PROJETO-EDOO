#include <iostream>
#include "Pessoa.hpp"
#include "Paciente.hpp"
using namespace std;

int main() {
    Paciente p("vitor", 25, "123.456.789-00", "987654321", "Rua Exemplo, 123", "Sem histórico médico", "Engenharia", "vitor@example.com", true, false);
    cout << p.getPaciente() << endl;
    cout << "Status do paciente: " << p.getStatus() << endl;
    p.validar();
    cout << "Status do paciente apos validacao: " << p.getStatus() << endl;
    return 0;
}