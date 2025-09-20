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
        mutex mtx;
        
    public:
        Hidrometro(Cano& entrada, Cano& saida, int volumeMax, float perdaAr)
        : entrada(entrada), saida(saida), volume(0), consumo(0), 
        perdaAr(perdaAr), dezenasLitros(0), centenasLitros(0) {}
        
        float getConsumo() {
            lock_guard<mutex> lock(mtx);
            return this->consumo;
        }

        Cano& getEntrada() const {
            return this->entrada;
        }

        int getDezenasLitros() {
            lock_guard<mutex> lock(mtx);
            return this->dezenasLitros;
        
        }
        int getCentenasLitros() {
            lock_guard<mutex> lock(mtx);
            return this->centenasLitros;
        }

        int getVolume() {
            lock_guard<mutex> lock(mtx);
            return this->volume;
        }

        void medir();
};