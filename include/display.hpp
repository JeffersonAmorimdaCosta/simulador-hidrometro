#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include "hidrometro.hpp"

using namespace std;

class Display {
    private:
        string caminhoImagemBase, nomeJanela;

    public:
        Display(string caminhoImagemBase, string nomeJanela) : caminhoImagemBase(caminhoImagemBase), 
        nomeJanela(nomeJanela) {cv::namedWindow(this->nomeJanela, cv::WINDOW_NORMAL);}
        
        cv::Mat gerarImagem(string consumo);
        void exibirImagem(cv::Mat& frame, int tempo);
        void salvarImagemJpeg(cv::Mat& frame, string caminho);
};