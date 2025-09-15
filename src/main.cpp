#include <iostream>
#include "Hidrometro.hpp"
#include "Cano.hpp"
#include "Display.hpp"
#include "Controlador.hpp"
#include "ImageManager.hpp"
#include <filesystem>
#include <string>

using namespace std;

int main(void) {
    float perdaAr = 0.1;
    int intervaloImagem = 2;
    int volumeMaximo = 999;

    Cano c1(1.5, 100);
    Cano c2(2.5, 100);
    Hidrometro h(c1, c2, volumeMaximo, perdaAr);
    Display d("../images/base/imagem_base_hidrometro.png");
    ImageManager im;
    Controlador controlador(intervaloImagem, h, d, im);

    cout << "Iniciando a simulacao..." << endl;

    controlador.iniciarControle();
    this_thread::sleep_for(chrono::seconds(30));
    controlador.pararControle();
    cout << "Concluindo a simulacao..." << endl;

    return 0;
}