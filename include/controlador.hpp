#pragma once

#include "hidrometro.hpp"
#include "display.hpp"
#include <random>
#include <atomic>
#include <thread>
#include <string>
#include <mutex>

using namespace std;

class Controlador {
    private:
        int intervaloImagem;
        Hidrometro& hidrometro;
        Display& display;
        atomic<bool> parar{false};
        thread tControle, tDisplay;
        mt19937 gen{(random_device{}())};

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

        string consumoFormatado() {
            return (to_string(this->hidrometro.getVolume()) + to_string(this->hidrometro.getCentenasLitros()) + to_string(this->hidrometro.getDezenasLitros()));
        }
        
        void controlar();
        
        public:
            Controlador(int intervaloImagem, Hidrometro& hidrometro, 
            Display& display) : intervaloImagem(intervaloImagem), 
            hidrometro(hidrometro), display(display) {}
            
            void iniciarControle();
            void pararControle();
            void exibicao();
};