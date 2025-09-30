#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include "hidrometro.hpp"
#include <mutex>

using namespace std;

class Display {
    private:
        string caminhoImagemBase, nomeJanela;
        static std::mutex guiMutex;

    public:
        Display(string caminhoImagemBase, string nomeJanela);

        cv::Mat gerarImagem(string consumo);
        void exibirImagem(cv::Mat& frame, int tempo);
        void salvarImagemJpeg(cv::Mat& frame, string caminho);
        void fecharJanela();
};