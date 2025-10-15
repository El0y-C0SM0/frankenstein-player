/***
 * @file Manager
 * @brief Essa classe é responsável por gerenciar as músicas do dispositivo
 * 
 * @date 2025-10-14
 * @author Pedro Gabriel
 *  
 ***/

#pragma once

#include "Loader.hpp"
#include "core/interfaces/IPlayable.hpp"


namespace core {

    class Manager {
    private:
        const Loader loader;

    public:

        /**
         * @brief Adiciona uma música
         * 
         * Essa classe é responsável por usar a entidade música para definir em qual
         * diretório a música deve ser salva 
         * 
         * @return true caso a música seja salva e false caso nenhuma operação seja feita (erro ou música já existe)
         */
        bool save(IPlayable &song);

        /**
         * @brief Carrega uma música salva no sistema
         * 
         * Usa os dados da objeto song para carregar a música
         * 
         * @return retorna um ponteiro para o arquivo carregado ou null_ptr caso a música não seja encontrada
         */
        void* load(IPlayable &song);

        /**
         * @brief Verifica que existe 
         * 
         * @param song instância da música no sistema
         * @return true caso seja encontrado no sistema
         * @return false caso exista o arquivo da música no sistema
         */
        bool exist(IPlayable &song);

    };

}

#pragma once