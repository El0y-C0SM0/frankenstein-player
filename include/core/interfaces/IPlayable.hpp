/**
 * @file IPlayable.hpp
 * @brief Arquivo de interface IPlayable
 *
 * Interface que define métodos para objetos que podem ser reproduzidos, como músicas ou vídeos.
 *
 * @ingroup interfaces
 * @author Eloy Maciel
 * @date 2025-10-11
 */

#pragma once

#include <vector>
#include <memory>

#include "core/interfaces/IPlayableObject.hpp"

namespace core {

    /**
     * @brief Interface para objetos que podem ser reproduzidos
     *
     * Define que uma classe reproduzível deve fornecer um método para
     * obter uma lista de objetos reproduzíveis.
     */
    class IPlayable {
    public:
        virtual ~IPlayable() = default;

        /**
         * @brief Obtém os objetos reproduzíveis
         * @return Vetor de ponteiros compartilhados para objetos reproduzíveis
         */
        virtual std::vector<std::shared_ptr<IPlayableObject>> getPlayableObjects() const = 0;
    };

}

#pragma once
