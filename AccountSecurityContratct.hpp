// contratos: seguranca de conta 

// protecao contra duplicaccao
#ifndef ACCOUNT_SECURITY_CONTRACT_HPP
#define  ACCOUNT_SECURITY_CONTRACT_HPP

#include <string>

/*o nosso trabalho eh todo local, ou seja a autenticacao
do usuario eh validade  por meio dos usuarios que existem 
no sistema*/


class AccountSecurityContract{
public:
     //construtor
     AccountSecurityContract();

     // funcao para verificar se o usuario existe no computador 

     bool authenticate(const std::string& username);

    // retorna true ou false 



};

#endif 
