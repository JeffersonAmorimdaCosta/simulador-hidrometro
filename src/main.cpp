#include <iostream>
#include "hidrometro.hpp"
#include "cano.hpp"
#include "display.hpp"
#include "controlador.hpp"

using namespace std;

int main(void) {
    float perda_ar = 0.1;
    int intervaloImagem = 10;

    Cano c1(1.5, 1);
    Cano c2(2.5, 1);
    Hidrometro h(c1, c2, 100, perda_ar);
    Display d;
    Controlador controlador(intervaloImagem, h, d);

    cout << "Iniciando a simulacao..." << endl;

    controlador.iniciarControle();
    this_thread::sleep_for(chrono::seconds(25));
    controlador.pararControle();
    cout << "Concluindo a simulacao..." << endl;

    return 0;
}