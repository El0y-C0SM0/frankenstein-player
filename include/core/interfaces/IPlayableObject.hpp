/**
 * @file IPlayableObject.hpp
 * @brief Interface para objetos reproduzíveis
 *
 * O Arquivo define a interface IPlayableObject, que representa objetos que podem ser reproduzidos,
 *
 * @ingroup interfaces
 * @author Eloy Maciel
 * @date 2025-10-11
 */

#pragma once

#include <string>

namespace core {

    /**
     * @brief Interface para objetos reproduzíveis
     *
     * Define que um objeto reproduzível deve fornecer um método para
     * obter o caminho do arquivo de áudio.
     */
    class IPlayableObject {
    public:
        virtual ~IPlayableObject() = default;

        /**
         * @brief Obtém o caminho do arquivo de áudio
         * @return Caminho do arquivo de áudio
         */
        virtual std::string getAudioFilePath() const = 0;
    };

}
