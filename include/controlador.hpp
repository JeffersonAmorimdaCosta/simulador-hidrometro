#pragma once

#include "hidrometro.hpp"
#include "display.hpp"
#include "ImageManager.hpp"
#include <random>
#include <atomic>
#include <thread>
#include <string>

using namespace std;

class Controlador {
    private:
        int intervaloImagem;
        Hidrometro& hidrometro;
        Display& display;
        ImageManager& im;
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

        string formatarConsumoTotal() {
            return (to_string(this->hidrometro.getVolume()) + to_string(this->hidrometro.getCentenasLitros()) + to_string(this->hidrometro.getDezenasLitros()));
        }
        
        void controlar();
        void exibicao();
        
    public:
        Controlador(int intervaloImagem, Hidrometro& hidrometro, 
        Display& display, ImageManager& im) : intervaloImagem(intervaloImagem), 
        hidrometro(hidrometro), display(display), im(im) {}
        
        void iniciarControle() {
            this->parar = false;
            tControle = thread(&Controlador::controlar, this);
            tDisplay = thread(&Controlador::exibicao, this);
        }

        void pararControle() {
            this->parar = true;
            if (tControle.joinable())
                this->tControle.join();
            if (tDisplay.joinable())
                this->tDisplay.join();
            
        }
};