#include "hidrometro.hpp"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

void Hidrometro::medir() {
    float aumento = this->entrada.getVazao();
    float aumento_final = this->entrada.getIsAr() ? aumento * this->perdaAr : aumento;

    lock_guard<mutex> lock(mtx);
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

string Hidrometro::getLeituraFormatada() {
    lock_guard<mutex> lock(mtx);

    int volumeLocal = this->volume;
    int centenas = this->centenasLitros;
    int dezenas = this->dezenasLitros;

    int litros = static_cast<int>(this->consumo);
    float parteDecimal = this->consumo - static_cast<float>(litros);
    int decimos = static_cast<int>(round(parteDecimal * 10.0f));

    if (decimos == 10) {
        decimos = 0;
        litros++;
        if (litros == 10) {
            litros = 0;
            dezenas++;
            if (dezenas == 10) {
                dezenas = 0;
                centenas++;
                if (centenas == 10) {
                    centenas = 0;
                    volumeLocal++;
                }
            }
        }
    }

    string leitura = to_string(volumeLocal) + to_string(centenas) + to_string(dezenas) +
                      to_string(litros) + to_string(decimos);

    while (leitura.size() < 6)
        leitura = "0" + leitura;

    return leitura;
}