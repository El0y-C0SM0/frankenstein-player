#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <chrono>
#include <iostream>
#include <thread>

int main() {
    std::cout << "🔊 Testando MP3 com MiniAudio..." << std::endl;

    ma_result result;
    ma_engine engine;

    // Inicializa o engine MiniAudio
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        std::cout << "❌ Falha ao inicializar MiniAudio: " << result << std::endl;
        return -1;
    }

    // Tenta carregar o MP3
    std::string mp3File = "/home/joao/Desktop/Pearl Jam - Black.mp3";
    std::cout << "📁 Carregando: " << mp3File << std::endl;

    ma_sound sound;
    result = ma_sound_init_from_file(&engine, mp3File.c_str(), 0, NULL, NULL, &sound);

    if (result == MA_SUCCESS) {
        std::cout << "✅ MP3 CARREGADO COM SUCESSO!" << std::endl;

        // Toca a música
        ma_sound_start(&sound);
        std::cout << "▶️  Reproduzindo... Pressione Enter para parar." << std::endl;

        // Espera entrada do usuário
        std::cin.get();

        // Para a reprodução
        ma_sound_stop(&sound);
        std::cout << "⏹️  Parado" << std::endl;

        ma_sound_uninit(&sound);
    } else {
        std::cout << "❌ Falha ao carregar MP3: " << result << std::endl;
        std::cout << "   Tente converter para WAV ou verifique o arquivo." << std::endl;
    }

    ma_engine_uninit(&engine);
    return 0;
}
