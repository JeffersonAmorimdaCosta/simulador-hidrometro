#include "hidrometro.hpp"
#include <cmath>

void Hidrometro::medir() {
    float aumento = this->entrada.getVazao();
    float aumento_final = this->entrada.getIsAr() ? aumento * this->perdaAr : aumento;
    this->consumo = round((this->consumo + aumento_final) * 10.0f) / 10.0f;

    while (this->consumo >= 10) {
        this->dezenasLitros++;
        this->consumo -= 10.0f;
    }

    while (this->dezenasLitros >= 10) {
        this->centenasLitros++;
        this->dezenasLitros -= 10.0f;
    }

    while (this->centenasLitros >= 10) {
        this->volume++;
        this->centenasLitros -= 10.0f;
    }
}