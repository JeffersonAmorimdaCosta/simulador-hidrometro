#pragma once

#include "hidrometro.hpp"
#include "display.hpp"
#include <random>
#include <atomic>
#include <thread>
#include <string>
#include <mutex>
#include <vector>
#include <filesystem>

using namespace std;

class Controlador {
    private:
        int intervaloImagem;
        Hidrometro& hidrometro;
        Display& display;
        atomic<bool> parar{false};
        thread tControle, tDisplay;
        mt19937 gen{(random_device{}())};
        filesystem::path diretorioSaida;
        bool salvarImagens;
        int proximoVolumeSnapshot{1};

        float gerarVazaoAleatoria() {
            uniform_real_distribution<float> dist(8, 15);
            float valor = dist(gen);
            valor = round(valor * 10.0f) / 10.0f;
            return valor;
        }

        bool gerarArAleatorio() {
            uniform_int_distribution<int> dist(1, 10);
            int valor = dist(gen);

            return valor > 8 ? true : false;
        }

        string consumoFormatado();

        void controlar();
        void exibicao();

        public:
            Controlador(int intervaloImagem, Hidrometro& hidrometro, 
            Display& display, filesystem::path diretorioSaida = {});
            ~Controlador();

            void iniciarControle();
            void pararControle();
};