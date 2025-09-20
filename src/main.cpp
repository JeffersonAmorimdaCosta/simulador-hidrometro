#include <iostream>
#include "Hidrometro.hpp"
#include "Cano.hpp"
#include "Display.hpp"
#include "Controlador.hpp"
#include <filesystem>
#include <string>

using namespace std;

int main(void) {
    float perdaAr = 0.1;
    int intervaloImagemMs = 70;
    int volumeMaximo = 999;
    int intervaloVazao = 50;

    int tempoExecucaoSeg = 30;

    Cano c1(1.5, intervaloVazao);
    Cano c2(2.5, intervaloVazao);
    Hidrometro h(c1, c2, volumeMaximo, perdaAr);
    Display d("../images/base/imagem_base_hidrometro.png", "Hidrometro");
    Controlador controlador(intervaloImagemMs, h, d);

    cout << "Iniciando a simulacao..." << endl;

    controlador.iniciarControle();

    thread tPrincipal([&]() {
        this_thread::sleep_for(chrono::seconds(tempoExecucaoSeg));
        controlador.pararControle();
    });

    controlador.exibicao();

    if (tPrincipal.joinable())
        tPrincipal.join();

    cout << "Concluindo a simulacao..." << endl;

    return 0;
}