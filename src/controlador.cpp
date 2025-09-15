#include "Controlador.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <filesystem>

using namespace std;

void Controlador::controlar() {
    int atualizacao = this->hidrometro.getEntrada().getTempoMiliseg();

    while (!this->parar) {
        this_thread::sleep_for(chrono::milliseconds(atualizacao));
        float vazaoAleatoria = this->gerarVazaoAleatoria();

        if (this->gerarArAleatorio())
            this->hidrometro.getEntrada().setIsAr(true);
        else
            this->hidrometro.getEntrada().setIsAr(false);

        string agua_ou_ar = this->hidrometro.getEntrada().getIsAr() ? "(ar)" : " "; 

        this->hidrometro.getEntrada().setVazao(vazaoAleatoria);

        // cout << "Vazao atual: " << vazaoAleatoria << agua_ou_ar << endl;
        // cout << "Dezenas | Centenas de Litros: " << this->hidrometro.getCentenasLitros()
        // << '|' << this->hidrometro.getDezenasLitros() << endl;
        // cout << "Volume: " << this->hidrometro.getVolume() << " m3" << endl << endl;

        this->hidrometro.medir();
    }
}

void Controlador::exibicao() {
    while (!this->parar) {
        int sequencia = this->display.getSequencia();
        string caminhoRel = "../images/medicoes_202311250016/";
        string caminhoAbs = filesystem::absolute(caminhoRel + to_string(sequencia) + ".png").string();

        this->display.gerarImagem(this->formatarConsumoTotal());
        auto p = this->im.abrirImagem(caminhoAbs);
        this_thread::sleep_for(chrono::seconds(this->intervaloImagem));
        this->im.fecharImagem(p);
    }
}