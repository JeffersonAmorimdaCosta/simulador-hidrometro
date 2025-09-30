#include "cano.hpp"

Cano::Cano(float bitola, int tempoMiliseg)
    : bitola(bitola), vazao(0.0f), isAr(false) {
    this->tempoMiliseg = tempoMiliseg > 0 ? tempoMiliseg : 100;
}

float Cano::getVazao() {
    lock_guard<mutex> lock(mtxVazao);
    return this->vazao;
}

bool Cano::getIsAr() {
    return this->isAr.load();
}

void Cano::setVazao(float vazao) {
    lock_guard<mutex> lock(mtxVazao);
    this->vazao = vazao;
}

void Cano::setIsAr(bool isAr) {
    this->isAr.store(isAr);
}
