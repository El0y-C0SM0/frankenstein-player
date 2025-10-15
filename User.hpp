#ifndef USER_HPP
#define USER_HPP

#include "Entity.hpp"
#include <string>
#include <vector>

namespace core {

class User : public Entity {
private:
    std::string _name;
    std::string _homeDirectory; // diretorio do user
    static User* _usuarioAtual;
    static std::vector<User> _usuarios;

public:
    // Getters e Setters 
    std::string getName() const;
    void setName(const std::string& name);

    std::string getHomeDirectory() const;
    void setHomeDirectory(const std::string& directory);


    
    // funcao usuarios igual herdada entity
    bool operator==(const Entity& other) const override;
};

} 

#endif
