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
        Cano(float bitola, int tempoMiliseg);

        float getBitola() const {return this->bitola;}

        float getVazao();

        int getTempoMiliseg() const {
            return this->tempoMiliseg;
        }

        bool getIsAr();

        void setVazao(float vazao);

        void setIsAr(bool isAr);
};