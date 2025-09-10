#include <iostream>
#include "hidrometro.hpp"
#include "cano.hpp"
#include "display.hpp"
#include "controlador.hpp"

using namespace std;

int main(void) {
    float perdaAr = 0.1;
    int intervaloImagem = 10;
    int volumeMaximo = 999;

    Cano c1(1.5, 1);
    Cano c2(2.5, 1);
    Hidrometro h(c1, c2, volumeMaximo, perdaAr);
    Display d;
    Controlador controlador(intervaloImagem, h, d);

    cout << "Iniciando a simulacao..." << endl;

    controlador.iniciarControle();
    this_thread::sleep_for(chrono::seconds(26));
    controlador.pararControle();
    cout << "Concluindo a simulacao..." << endl;

    return 0;
}