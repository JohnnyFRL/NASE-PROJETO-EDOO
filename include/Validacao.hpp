#ifndef VALIDACAO_HPP
#define VALIDACAO_HPP

#include <string>
#include <vector>
#include "Usuario.hpp"

using namespace std;

class Validacao {
public:

    // [T1] Valida nome completo (apenas letras/acentos/espacos,
    //      minimo de nome + sobrenome).
    static bool validarNome(const string& nome, string& erro);

    // [T2] Valida senha forte: minimo 8 chars, maiuscula,
    //      minuscula, digito e caractere especial.
    static bool validarSenhaForte(const string& senha, string& erro);

    // [T3] Verifica se o login nao existe entre os usuarios
    //      ja cadastrados (unicidade) e tamanho minimo.
    static bool validarLoginUnico(const string& login,
                                  const vector<Usuario*>& usuarios,
                                  string& erro);

    // [T4] Valida formato de e-mail (local@dominio.com).
    static bool validarEmail(const string& email, string& erro);

    // [T5] Valida telefone:
    //      aceita (XX) XXXXX-XXXX
    static bool validarTelefone(const string& telefone, string& erro);
};

#endif
