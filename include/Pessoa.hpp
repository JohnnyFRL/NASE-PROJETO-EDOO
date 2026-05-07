#ifndef PESSOA_HPP
#define PESSOA_HPP

#include <string>
using namespace std;

class Pessoa{
private:
    int id; // ID no banco de dados
    int idade;
    string nome;
    string cpf;
    string telefone;
    string endereco;

public:
    Pessoa(const string& nome, int idade, const string& cpf, const string& telefone, const string& endereco);
    Pessoa(int id, const string& nome, int idade, const string& cpf, const string& telefone, const string& endereco);
    virtual ~Pessoa() = default;

    string getPessoa() const;
    int getId() const;
    void setId(int novoId);
    int getIdade() const;
    const string& getNome() const;
    const string& getCpf() const;
    const string& getTelefone() const;
    const string& getEndereco() const;
    void setNome(const string& novoNome);
    void setIdade(int novaIdade);
    void setTelefone(const string& novoTelefone);
    void setEndereco(const string& novoEndereco);
};

#endif