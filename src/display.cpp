#include "display.hpp"
// #include <cairomm/cairomm.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <atomic>

using namespace std;

std::mutex Display::guiMutex;

Display::Display(string caminhoImagemBase, string nomeJanela)
    : caminhoImagemBase(std::move(caminhoImagemBase)), nomeJanela(std::move(nomeJanela)) {
    std::lock_guard<std::mutex> lock(guiMutex);
    cv::namedWindow(this->nomeJanela, cv::WINDOW_NORMAL);
}

cv::Mat Display::gerarImagem(string consumo) {
    cv::Mat imagemBase = cv::imread(this->caminhoImagemBase);

    if (imagemBase.empty()) {
        cerr << "ERRO: Nao foi possivel carregar a imagem" << endl;
        return cv::Mat();
    }

    int qntDigitos = 6;
    string valorDisplay = string(qntDigitos - consumo.size(), '0') + consumo;

    cv::Mat frame = imagemBase.clone();

    int pos_x = 408, pos_y = 390;

    int tipoFonte = cv::FONT_HERSHEY_PLAIN;
    double escalaFonte = 2.8;
    cv::Scalar corTexto(0, 0, 0);
    int espessura = 4;

    int cont = 0;

    for (auto digito : valorDisplay) {
        if (cont >= 4)
            corTexto = cv::Scalar(0, 0, 255);

        cv::Point posicaoTexto(pos_x, pos_y);
        cv::putText(frame, string(1, digito), posicaoTexto, tipoFonte, escalaFonte, corTexto, espessura, cv::LINE_AA);
        pos_x += 38;
        cont++;
    }

    cv::resizeWindow(this->nomeJanela, 800, 700);
    cv::moveWindow(this->nomeJanela, 0, 0);

    return frame;
}

void Display::exibirImagem(cv::Mat& frame, int tempo) {
    std::lock_guard<std::mutex> lock(guiMutex);
    cv::imshow(this->nomeJanela, frame);
    cv::waitKey(tempo);
}

void Display::salvarImagemJpeg(cv::Mat& frame, string caminho) {
    if (!cv::imwrite(caminho, frame)) {
        cerr << "ERRO: nao foi possivel salvar a imagem em " << caminho << endl;
    }
}

void Display::fecharJanela() {
    std::lock_guard<std::mutex> lock(guiMutex);
    cv::destroyWindow(this->nomeJanela);
}