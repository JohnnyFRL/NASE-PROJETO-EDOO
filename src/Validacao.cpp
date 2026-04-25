#include "Validacao.hpp"
#include <cctype>   
#include <iostream>

using namespace std;

// ============================================================
// [T1] Validacao de Nome
// Regras:
//   - Minimo 3 caracteres no total.
//   - Apenas letras (incluindo caracteres acentuados >= 128),
//     espacos simples permitidos.
//   - Deve conter pelo menos duas palavras (nome + sobrenome).
//   - Nao pode comecar nem terminar com espaco.
//   - Nao pode ter espacos consecutivos.
// ============================================================
bool Validacao::validarNome(const string& nome, string& erro) {

    if (nome.length() < 3) {
        erro = "Nome muito curto (minimo 3 caracteres).";
        return false;
    }

    // Nao pode comecar ou terminar com espaco
    if (nome[0] == ' ' || nome[nome.length() - 1] == ' ') {
        erro = "Nome nao pode comecar nem terminar com espaco.";
        return false;
    }

    for (int i = 0; i < (int)nome.length(); i++) {
        unsigned char c = (unsigned char)nome[i];

        // Aceita letras do alfabeto basico, caracteres acentuados (>= 128) e espacos
        bool eLetra    = (isalpha(c) != 0);
        bool eAcentuado = (c >= 128);
        bool eEspaco   = (c == ' ');

        if (!eLetra && !eAcentuado && !eEspaco) {
            erro = "Nome deve conter apenas letras e espacos (sem numeros ou simbolos).";
            return false;
        }

        // Espacos consecutivos sao invalidos
        if (eEspaco && i + 1 < (int)nome.length() && nome[i + 1] == ' ') {
            erro = "Nome nao pode conter espacos consecutivos.";
            return false;
        }
    }

    // Verifica se existe pelo menos nome + sobrenome (uma palavra apos um espaco)
    bool achouEspaco       = false;
    bool achouPalavraApos  = false;

    for (int i = 0; i < (int)nome.length(); i++) {
        if (nome[i] == ' ') {
            achouEspaco = true;
        } else if (achouEspaco) {
            achouPalavraApos = true;
            break;
        }
    }

    if (!achouEspaco || !achouPalavraApos) {
        erro = "Informe nome e sobrenome (pelo menos duas palavras).";
        return false;
    }

    return true;
}

// ============================================================
// [T2] Validacao de Senha Forte
// Regras:
//   - Minimo 8 caracteres.
//   - Pelo menos 1 letra maiuscula.
//   - Pelo menos 1 letra minuscula.
//   - Pelo menos 1 digito (0-9).
//   - Pelo menos 1 caractere especial do conjunto: !@#$%^&*()_+-=[]{}|;':",./<>?
// ============================================================
bool Validacao::validarSenhaForte(const string& senha, string& erro) {

    if (senha.length() < 8) {
        erro = "Senha deve ter pelo menos 8 caracteres.";
        return false;
    }

    bool temMaiuscula = false;
    bool temMinuscula = false;
    bool temDigito    = false;
    bool temEspecial  = false;

    // Conjunto de caracteres especiais permitidos
    string especiais = "!@#$%^&*()_+-=[]{}|;':\",./<>?";

    for (int i = 0; i < (int)senha.length(); i++) {
        unsigned char c = (unsigned char)senha[i];

        if (isupper(c)) {
            temMaiuscula = true;
        } else if (islower(c)) {
            temMinuscula = true;
        } else if (isdigit(c)) {
            temDigito = true;
        } else {
            // Verifica se o caractere esta no conjunto de especiais
            for (int j = 0; j < (int)especiais.length(); j++) {
                if (senha[i] == especiais[j]) {
                    temEspecial = true;
                    break;
                }
            }
        }
    }

    if (!temMaiuscula) {
        erro = "Senha deve ter pelo menos uma letra maiuscula (ex: A, B, C...).";
        return false;
    }
    if (!temMinuscula) {
        erro = "Senha deve ter pelo menos uma letra minuscula (ex: a, b, c...).";
        return false;
    }
    if (!temDigito) {
        erro = "Senha deve ter pelo menos um numero (ex: 1, 2, 3...).";
        return false;
    }
    if (!temEspecial) {
        erro = "Senha deve ter pelo menos um caractere especial (ex: !, @, #, $...).";
        return false;
    }

    return true;
}

// ============================================================
// [T3] Validacao de Login Unico
// Regras:
//   - Minimo 3 caracteres.
//   - Nao pode conter espacos.
//   - Nao pode ja existir entre os usuarios cadastrados.
// ============================================================
bool Validacao::validarLoginUnico(const string& login,
                                  const vector<Usuario*>& usuarios,
                                  string& erro) {

    if (login.length() < 3) {
        erro = "Login deve ter pelo menos 3 caracteres.";
        return false;
    }

    // Login nao pode ter espacos
    for (int i = 0; i < (int)login.length(); i++) {
        if (login[i] == ' ') {
            erro = "Login nao pode conter espacos.";
            return false;
        }
    }

    // Verificar unicidade percorrendo manualmente o vetor
    for (int i = 0; i < (int)usuarios.size(); i++) {
        if (usuarios[i]->getLogin() == login) {
            erro = "Login \"" + login + "\" ja esta em uso. Escolha outro.";
            return false;
        }
    }

    return true;
}

// ============================================================
// [T4] Validacao de E-mail
// Regras:
//   - Deve conter exatamente um '@'.
//   - A parte local (antes do '@') nao pode estar vazia.
//   - O dominio (apos o '@') nao pode estar vazio.
//   - O dominio deve conter pelo menos um ponto.
//   - O dominio nao pode comecar nem terminar com ponto.
//   - Nao pode haver pontos consecutivos.
// ============================================================
bool Validacao::validarEmail(const string& email, string& erro) {

    if (email.length() == 0) {
        erro = "Email nao pode ser vazio.";
        return false;
    }

    // Contar e localizar o '@'
    int posArroba = -1;
    int qtdArroba = 0;

    for (int i = 0; i < (int)email.length(); i++) {
        if (email[i] == '@') {
            posArroba = i;
            qtdArroba++;
        }
    }

    if (qtdArroba != 1) {
        erro = "Email deve conter exatamente um '@'.";
        return false;
    }

    // Parte local (antes do '@') nao pode ser vazia
    if (posArroba == 0) {
        erro = "Email invalido: a parte local (antes do '@') nao pode estar vazia.";
        return false;
    }

    // Dominio (apos o '@')
    string dominio = email.substr(posArroba + 1);

    if (dominio.length() == 0) {
        erro = "Email invalido: o dominio (apos o '@') nao pode estar vazio.";
        return false;
    }

    // Dominio nao pode comecar ou terminar com ponto
    if (dominio[0] == '.' || dominio[dominio.length() - 1] == '.') {
        erro = "Email invalido: o dominio nao pode comecar nem terminar com ponto.";
        return false;
    }

    // Dominio deve ter pelo menos um ponto e sem pontos consecutivos
    bool temPonto = false;

    for (int i = 0; i < (int)dominio.length(); i++) {
        if (dominio[i] == '.') {
            temPonto = true;
            if (i + 1 < (int)dominio.length() && dominio[i + 1] == '.') {
                erro = "Email invalido: dominio nao pode ter pontos consecutivos.";
                return false;
            }
        }
    }

    if (!temPonto) {
        erro = "Email invalido: o dominio deve conter pelo menos um ponto (ex: .com, .br).";
        return false;
    }

    return true;
}

// ============================================================
// [T5] Validacao de Telefone Brasileiro
// Regras:
//   - Aceita apenas digitos, parenteses, hifens e espacos.
//   - Apos extrair somente os digitos, o total deve ser
//     10 (fixo) ou 11 (celular) digitos.
//   - O DDD (primeiros 2 digitos) deve estar entre 11 e 99.
//   - Para 11 digitos (celular), o 3o digito deve ser 9.
// ============================================================
bool Validacao::validarTelefone(const string& telefone, string& erro) {

    if (telefone.length() == 0) {
        erro = "Telefone nao pode ser vazio.";
        return false;
    }

    // Extrair somente os digitos e verificar caracteres invalidos
    string digitos = "";

    for (int i = 0; i < (int)telefone.length(); i++) {
        char c = telefone[i];
        if (c >= '0' && c <= '9') {
            digitos += c;
        } else if (c == '(' || c == ')' || c == '-' || c == ' ') {
            // caracteres de formatacao: ignorar
            continue;
        } else {
            erro = "Telefone contem caractere invalido: '" + string(1, c) + "'.";
            return false;
        }
    }

    // Deve ter 10 (fixo) ou 11 (celular) digitos
    int total = (int)digitos.length();
    if (total != 10 && total != 11) {
        erro = "Telefone invalido! Use (XX) XXXX-XXXX (fixo) ou (XX) 9XXXX-XXXX (celular).";
        return false;
    }

    // Valida o DDD (primeiros dois digitos): de 11 a 99
    int ddd = (digitos[0] - '0') * 10 + (digitos[1] - '0');
    if (ddd < 11 || ddd > 99) {
        erro = "DDD invalido. O DDD deve estar entre 11 e 99.";
        return false;
    }

    // Para celular (11 digitos), o 3o digito deve ser 9
    if (total == 11 && digitos[2] != '9') {
        erro = "Celular invalido: o 3o digito deve ser 9 (ex: (XX) 9XXXX-XXXX).";
        return false;
    }

    return true;
}
