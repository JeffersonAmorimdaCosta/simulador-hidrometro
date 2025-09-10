#include "controlador.hpp"
#include <chrono>
#include <thread>
#include <iostream>

using namespace std;

void Controlador::controlar() {
    this->parar = false;
    // int atualizacao = this->hidrometro.getEntrada().getTempoSegundos();
    int atualizacao = 100;

    while (!this->parar) {
        cout << "Consumo: " << this->hidrometro.getConsumo() << 'L' << endl;
        // this_thread::sleep_for(chrono::seconds(atualizacao));
        this_thread::sleep_for(chrono::milliseconds(atualizacao)); // Para apresentação
        float vazaoAleatoria = this->gerarVazaoAleatoria();
        this->hidrometro.getEntrada().setVazao(vazaoAleatoria);
        cout << "Vazao atual: " << vazaoAleatoria << endl;
        cout << "Dezenas | Centenas de Litros: " << this->hidrometro.getCentenasLitros()
        << '|' << this->hidrometro.getDezenasLitros() << endl;
        cout << "Volume: " << this->hidrometro.getVolume() << " m3" << endl << endl;
        this->hidrometro.medir();
    }
}