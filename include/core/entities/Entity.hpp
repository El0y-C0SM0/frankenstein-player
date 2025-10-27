/**
 * @file Entity.hpp
 * @brief Interface para entidades do sistema
 *
 * Interface que define caracteristicas essenciais para entidades do sistema.
 *
 * @author Eloy Maciel
 * @date 2025-10-09
 */

 #pragma once

#include "core/util/Datetime.hpp"

namespace core {

    /**
    * @brief Interface para entidades do sistema
    * Interface que define caracteristicas essenciais para entidades do sistema.
    */
    class Entity {
    protected:
        unsigned _id = 0; /**< ID da entidade */
        Datetime _dataCriacao; /** Data de criação */

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

        /**
         * @brief Obtém a data de criação
         * @return a data de criação da entidade
         */
        const Datetime getDataCriacao() const;

        /**
         * @brief Define a data de criação da entidade
         * @param date Data de criação
         */
        void setDataCriacao(Datetime date);

        /**
        * @brief Compara duas entidades
        * @param other Entidade a ser comparada
        * @return true se as entidades forem iguais, false caso contrário
        */
        virtual bool operator==(const Entity &other) const = 0;

        /**
        * @brief Compara duas entidades para desigualdade
        * @param other Entidade a ser comparada
        * @return true se as entidades forem diferentes, false caso contrário
        */
        virtual bool operator!=(const Entity &other) const = 0;
    };
} // namespace core
