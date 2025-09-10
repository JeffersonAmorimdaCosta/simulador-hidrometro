#pragma once

#include <atomic>
#include <mutex>

using namespace std;

class Cano {
    private:
        float bitola;
        // float pressao;
        int tempoSegundos;
        float vazao;
        atomic<bool> isAr;
        mutex mtxVazao;

    public:
        // Cano(float bitola, float pressao, float vazao, bool isAr) : 
        // bitola(bitola), pressao(pressao), vazao(vazao), isAr(isAr) {}

        Cano(float bitola, int tempoSegundos) : bitola(bitola), vazao(0), 
        isAr(false) {
            this->tempoSegundos = tempoSegundos >= 0 ? 5 : tempoSegundos;
        }
        
        float getBitola() const {return this->bitola;}
        // float getPressao() const {return this->pressao;}

        float getVazao() {
            // lock_guard<mutex> lock(mtxVazao);
            return this->vazao;
        }

        int getTempoSegundos() const {
            return this->tempoSegundos;
        }

        bool getIsAr() {
            return this->isAr;
        }

        void setVazao(float vazao) {
            lock_guard<mutex> lock(mtxVazao);
            this->vazao = vazao;
        }

        void setIsAr(bool isAr) {
            this->isAr = isAr;
        }
};