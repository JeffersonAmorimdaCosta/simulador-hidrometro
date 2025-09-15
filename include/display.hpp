#pragma once

#include <iostream>

using namespace std;

class Display {
    private:
        int sequencia;
        string caminhoImagemBase;

        void avancarSequencia() {this->sequencia = (sequencia % 999) + 1;}

    public:
        Display(string caminhoImagemBase) : caminhoImagemBase(caminhoImagemBase)
        , sequencia(1) {}

        void gerarImagem(string consumo);

        int getSequencia() const {return this->sequencia;}
        
};