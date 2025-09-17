#pragma once

#include <iostream>

using namespace std;

class Display {
    private:
        string caminhoImagemBase;

    public:
        Display(string caminhoImagemBase) : caminhoImagemBase(caminhoImagemBase) {}
        
        void gerarImagem(string consumo, string nomeImagem);
        void pngParaJpeg(string caminhoImagem);
};