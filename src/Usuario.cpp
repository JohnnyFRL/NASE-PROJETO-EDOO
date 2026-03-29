#include "Usuario.hpp"

Usuario::Usuario(string login, string senha){
    this->login = login;
    this->senha = senha;
}

string Usuario::getLogin(){
    return login;
}

bool Usuario::autenticar(string senha){
    return this-> senha == senha;
}