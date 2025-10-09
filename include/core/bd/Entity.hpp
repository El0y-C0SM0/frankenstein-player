/**
 * @file Entity.hpp
 * @brief Interface para entidades do sistema
 *
 * Interface que define caracteristicas essenciais para entidades do sistema.
 *
 * @author Eloy Maciel
 * @date 2025-10-09
 */

#ifndef ENTITY_HPP
#define ENTITY_HPP

namespace core {

    /**
     * @brief Interface para entidades do sistema
     * Interface que define caracteristicas essenciais para entidades do sistema.
     */
    class Entity {
    public:
        virtual ~Entity() = default;

        /**
         * @brief Obtém o ID da entidade
         * @return ID da entidade
         */
        unsigned getId() const;

        /**
         * @brief Define o ID da entidade
         * @param id Novo ID da entidade
         */
        void setId(unsigned id);
    };

}

#endif
