/**
 * @file Loader.hpp
 * @brief Classe responsável por fazer o carregamento das músicas no diretório correto
 * 
 * Essa classe é responsável por fazer a interface técnica do sistema com as músicas salvas nos diretórios correspondentes
 *  
 * @date 2025-10-14
 * @author Pedro Gabriel
 */

#pragma once

#include <filesystem>

#include "core/interfaces/IPlayable.hpp"
#include "core/entities/Song.hpp"
#include "core/entities/EntitiesFWD.hpp"

 namespace core {

    class Loader {
        private:
            std::shared_ptr<User> _user;

        public:
        
        /**
         * @brief Construtor padrão do Loader
         * @param user: usuário autenticado no sistema
         * 
         */
        Loader(std::shared_ptr<User> user);

         /**
         * @brief Destrutor padrão do Loader
         */
        ~Loader();

        /**
         * @brief Verifica que uma música exoste no diretório com base nos dados passados 
         * @param filePath o caminho do diretório em que a música está armazenado
         * @return true caso encontre e false caso as informações não batem com nenhum item
         * 
         */
        bool exists(const IPlayable song);

        /**
         * @brief Procurar uma música no sistema 
         * @param filePath o caminho do diretório em que a música está armazenado
         * @return shared_ptr para o objeto da música
         * 
         */
        std::shared_ptr<IPlayable> search(const IPlayable song);

        /**
         * @brief Carregar uma música do sistema 
         * @param filePath o caminho do diretório em que a música está armazenado
         * @return ponteiro para o objeto da música ou null_ptr caso não exista
         * 
         */
        void* download(std::string filePath);


        /**
         * @brief Salva uma música no sistema 
         * @param song objeto da música
         * @param filePath o caminho do diretório em que a música está armazenado
         * @return shared_ptr para o objeto da música
         * 
         */
        bool load(const IPlayable &song, std::string filePath);
            
    };

 }

#pragma once