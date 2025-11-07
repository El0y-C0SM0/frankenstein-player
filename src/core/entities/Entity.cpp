/**
 * @file Entity.cpp
 * @brief Implementação da base de entidades
 *
 * @ingroup entities
 * @author Eloy Maciel
 * @date 2025-11-06
 */


#include "core/entities/Entity.hpp"


namespace core {
    unsigned Entity::getId() const {
        return _id;
    }

    void Entity::setId(unsigned id) {
        _id = id;
    }

    const Datetime Entity::getDataCriacao() const {
        return _dataCriacao;
    }

    void Entity::setDataCriacao(Datetime date) {
        _dataCriacao = date;
    }

    bool Entity::operator==(const Entity& other) const {
        return this->getId() == other.getId();
    }

    bool Entity::operator!=(const Entity& other) const {
        return !(*this == other);
    }
}  // namespace core
