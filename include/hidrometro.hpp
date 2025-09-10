#pragma once

#include "cano.hpp"
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>

using namespace std;

class Hidrometro {
    private:
        Cano& entrada;
        Cano& saida;
        int volumeMax;
        float consumo, perdaAr;
        int dezenasLitros, centenasLitros, volume;
        
    public:
        Hidrometro(Cano& entrada, Cano& saida, int volumeMax, float perdaAr)
        : entrada(entrada), saida(saida), volume(0), consumo(0), 
        perdaAr(perdaAr), dezenasLitros(0), centenasLitros(0) {}
        
        float getConsumo() {
            return this->consumo;
        }

        Cano& getEntrada() const {
            return this->entrada;
        }

        int getDezenasLitros() const {return this->dezenasLitros;}
        int getCentenasLitros() const {return this->centenasLitros;}
        int getVolume() const {return this->volume;}

        void medir();
};