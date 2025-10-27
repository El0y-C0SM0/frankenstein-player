/**
 * @file Datetime.hpp
 * @brief Define um utilitário para representar datas
 *
 * Esta classe representa datas
 *
 * @author Pedro Gabriel
 * @date 2025-10-14
 */

#pragma once

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>

namespace core {
    class Datetime {
        private:
            
            std::chrono::system_clock::time_point timePoint;

        public:

        /**
         * @brief Construtor padrão de Datetime com a data atual
         */
        Datetime();

        /**
         * @brief Construtor usando uma string
         * 
         * @param dateTimeStr data no formato DD-MM-YYYY
         */
        Datetime(const std::string& datetimeStr);

        /**
         * @brief Representação padrão de uma data
         */
        const std::string toString() const;

        /**
         * @brief Sobrecarga do operador equals
         * 
         * Considera apenas a data
         */
        bool operator==(const Datetime& other) const;

        /**
         * @brief Sobrecarga do operador not equals
         * 
         * Considera apenas a data
         */
        bool operator!=(const Datetime& other) const;

        /**
         * @brief Verifica a cronologia de duas datas
         * 
         * @return true se a data desse objeto é de antes do other e false caso seja de depois ou seja igual
         */
        bool isBefore(const Datetime& other) const;

        /**
         * @brief Verifica a cronologia de duas datas
         * 
         * @return true se a data desse objeto é depois de other e false caso contrário
         */
        bool isAfter(const Datetime& other) const;
    };
}

#pragma once