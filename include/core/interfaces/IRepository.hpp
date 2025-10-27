/**
 * @file IRepository.hpp
 * @brief Interface para repositório de dados
 * @ingroup bd
 *
 * Interface que define operações essenciais para um repositório de dados, incluindo métodos para salvar, remover, atualizar e buscar entidades.
 *
 * @author Eloy Maciel
 * @date 2025-10-09
 */

#pragma once

#include <vector>
#include <string>
#include <memory>


namespace core {

    /**
        * @brief Interface template para operações de repositório
        * @tparam T Tipo da entidade gerenciada pelo repositório
        *
        * @details
        * Esta interface define o contrato para operações CRUD básicas que
        * todos os repositórios devem implementar, independentemente da
        * tecnologia de persistência utilizada.
        */
    template <typename T>
    class IRepository {
    protected:

        /**
         * @brief Insere uma nova entidade no repositório
         * @param entity Entidade a ser inserida
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        virtual bool insert(const T &entity) = 0;

        /**
         * @brief Atualiza uma entidade existente no repositório
         * @param entity Entidade a ser atualizada
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        virtual bool update(const T &entity) = 0;
    public:
        virtual ~IRepository() = default;

        /**
         * @brief Salva uma entidade no repositório (inserção ou atualização)
         * @param entity Entidade a ser salva
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        virtual bool save(T &entity) = 0;

        /**
         * @brief Remove uma entidade do repositório pelo ID
         * @param id ID da entidade a ser removida
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        virtual bool remove(unsigned id) = 0;

        /**
         * @brief Busca uma entidade pelo ID
         * @param id ID da entidade a ser buscada
         * @return Ponteiro para a entidade encontrada, ou nullptr se não encontrada
         */
        virtual std::shared_ptr<T> findById(unsigned id) const = 0;

        /**
         * @brief Obtém todas as entidades do repositório
         * @return Vetor contendo todas as entidades
         */
        virtual std::vector<std::shared_ptr<T>> getAll() const = 0;

        /**
         * @brief Busca entidades por um campo específico
         * @param field Nome do campo a ser filtrado
         * @param value Valor do campo para filtro
         * @return Vetor contendo as entidades que correspondem ao critério
         */
        virtual std::vector<std::shared_ptr<T>> findBy(
            const std::string& field,
            const std::string& value
        ) const = 0;

        /**
         * @brief Verifica se uma entidade existe no repositório
         * @param id ID da entidade a ser verificada
         * @return true se a entidade existe, false caso contrário
         */
        virtual bool exists(unsigned id) const = 0;

        /**
         * @brief Obtém o número total de entidades no repositório
         * @return Quantidade de entidades
         */
        virtual size_t count() const = 0;

        /**
         * @brief Remove todas as entidades do repositório
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        virtual bool removeAll() = 0;
    };
}
