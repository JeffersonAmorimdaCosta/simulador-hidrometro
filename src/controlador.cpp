#include "controlador.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <filesystem>
#include <cstdio>
#include <sstream>

using namespace std;

Controlador::Controlador(int intervaloImagem, Hidrometro& hidrometro, Display& display, filesystem::path diretorioSaida)
    : intervaloImagem(intervaloImagem),
      hidrometro(hidrometro),
      display(display),
      diretorioSaida(std::move(diretorioSaida)),
      salvarImagens(!this->diretorioSaida.empty()) {}

Controlador::~Controlador() {
    pararControle();
    display.fecharJanela();
}

string Controlador::consumoFormatado() {
    return this->hidrometro.getLeituraFormatada();
}

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
    while (!this->parar.load()) {
        cv::Mat frame = this->display.gerarImagem(this->consumoFormatado());

        if (frame.empty()) {
            cerr << "Falha ao gerar a imagem!" << endl;
            return;
        }

        this->display.exibirImagem(frame, this->intervaloImagem);

        if (salvarImagens && this->hidrometro.getVolume() >= proximoVolumeSnapshot) {
            filesystem::create_directories(diretorioSaida);
            std::stringstream oss;
            oss << setw(2) << setfill('0') << proximoVolumeSnapshot << ".jpeg";
            auto caminho = diretorioSaida / oss.str();
            this->display.salvarImagemJpeg(frame, caminho.string());
            ++proximoVolumeSnapshot;
        }
    }
}

void Controlador::iniciarControle() {
    this->parar = false;
    tControle = thread(&Controlador::controlar, this);
    tDisplay = thread(&Controlador::exibicao, this);
}

void Controlador::pararControle() {
    this->parar = true;
    if (tControle.joinable())
        this->tControle.join();
    if (tDisplay.joinable())
        this->tDisplay.join();
}