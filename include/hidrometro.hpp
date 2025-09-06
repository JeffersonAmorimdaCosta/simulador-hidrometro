#pragma once

class Hidrometro {
    private:
        int valor;
    public:
        Hidrometro(short int valor) : valor(valor) {}

        int get_valor() {return this->valor;}
};