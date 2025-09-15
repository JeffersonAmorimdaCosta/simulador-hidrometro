#pragma once

#include <atomic>
#include <mutex>

using namespace std;

class Cano {
    private:
        float bitola;
        int tempoMiliseg;
        float vazao;
        atomic<bool> isAr;
        mutex mtxVazao;

    public:
        Cano(float bitola, int tempoMiliseg) : bitola(bitola), vazao(0), 
        isAr(false) {
            this->tempoMiliseg = tempoMiliseg > 0 ? tempoMiliseg : 100;
        }
        
        float getBitola() const {return this->bitola;}

        float getVazao() {
            return this->vazao;
        }

        int getTempoMiliseg() const {
            return this->tempoMiliseg;
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