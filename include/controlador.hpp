#pragma once

#include "hidrometro.hpp"
#include "display.hpp"
#include <random>
#include <atomic>
#include <thread>

using namespace std;

class Controlador {
    private:
        int intervaloImagem;
        Hidrometro& hidrometro;
        Display& display;
        atomic<bool> parar{false};
        thread t;
        mt19937 gen{(random_device{}())};

        float gerarVazaoAleatoria() {
            uniform_real_distribution<float> dist(8, 15);
            float valor = dist(gen);
            valor = round(valor * 10.0f) / 10.0f;
            return valor;
        }
        
        void controlar();
        
    public:
        Controlador(int intervaloImagem, Hidrometro& hidrometro, 
        Display& display) : intervaloImagem(intervaloImagem), 
        hidrometro(hidrometro), display(display) {}
        
        void iniciarControle() {
            this->parar = false;
            t = thread(&Controlador::controlar, this);
        }

        void pararControle() {
            this->parar = true;
            if (t.joinable())
                t.join();
        }
};