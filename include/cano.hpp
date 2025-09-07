#pragma once

class Cano {
    private:
        float bitola;
        float pressao;
        float vazao;
    public:
        Cano(float bitola, float pressao, float vazao) : bitola(bitola), pressao(pressao), vazao(vazao) {}
        
        float getBitola() const {return this->bitola;}
        float getPressao() const {return this->pressao;}
        float getVazao() const {return this->vazao;}

        void setVolume(float volume) {this->vazao = vazao;}
};