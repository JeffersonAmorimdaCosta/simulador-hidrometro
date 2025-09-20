#include "controlador.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <filesystem>
#include <cstdio>
#include <sstream>

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
        this->hidrometro.medir();
    }
}

void Controlador::exibicao() {
    string caminhoRel = "../images/medicoes_202311250016/";
    int m3 = 1;
    string caminhoAbs;
    bool salvar;
    while (!this->parar.load()) {
        stringstream oss;
        oss << setw(2) << setfill('0') << m3 << ".jpeg";
        caminhoAbs = filesystem::absolute(caminhoRel + oss.str()).string();

        salvar = this->hidrometro.getVolume() == m3;

        cv::Mat frame = this->display.gerarImagem(this->consumoFormatado());

        if (frame.empty()) {
            cerr << "Falha ao gerar a imagem!" << endl;
            return;
        }

        this->display.exibirImagem(frame, this->intervaloImagem);
        
        if (salvar) {
            this->display.salvarImagemJpeg(frame, caminhoAbs);
            m3++;
        }
    }    
}

void Controlador::iniciarControle() {
    this->parar = false;
    tControle = thread(&Controlador::controlar, this);
}

void Controlador::pararControle() {
    this->parar = true;
    if (tControle.joinable())
        this->tControle.join();
}