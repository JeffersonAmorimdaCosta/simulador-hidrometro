#include <iostream>
#include "hidrometro.hpp"
#include "cano.hpp"
#include "display.hpp"
#include "controlador.hpp"
#include <filesystem>
#include <string>
#include <vector>

using namespace std;

int main(void) {
    float perdaAr = 0.1;
    int intervaloImagemMs = 70;
    int volumeMaximo = 999;
    int intervaloVazao = 50;

    int tempoExecucaoSeg = 30;

    vector<unique_ptr<Cano>> canosEntrada;
    vector<unique_ptr<Cano>> canosSaida;
    vector<unique_ptr<Hidrometro>> hidrometros;
    vector<unique_ptr<Display>> displays;
    vector<unique_ptr<Controlador>> controladores;

    filesystem::path diretorioSaida = filesystem::current_path() / "medicoes";
    filesystem::create_directories(diretorioSaida);

    int quantidadeSimuladores = 5;

    cout << "Iniciando " << quantidadeSimuladores << " simuladores..." << endl;

    for (int i = 0; i < quantidadeSimuladores; ++i) {
        canosEntrada.emplace_back(make_unique<Cano>(1.5f + i * 0.1f, intervaloVazao));
        canosSaida.emplace_back(make_unique<Cano>(2.0f + i * 0.1f, intervaloVazao));
        hidrometros.emplace_back(make_unique<Hidrometro>(*canosEntrada.back(), *canosSaida.back(), volumeMaximo, perdaAr));

        string nomeJanela = "Hidrometro-" + to_string(i + 1);
        filesystem::path dirInstancia = diretorioSaida / ("simulador_" + to_string(i + 1));
        displays.emplace_back(make_unique<Display>("../images/base/imagem_base_hidrometro.png", nomeJanela));
        controladores.emplace_back(make_unique<Controlador>(intervaloImagemMs, *hidrometros.back(), *displays.back(), dirInstancia));
    }

    for (auto& controlador : controladores) {
        controlador->iniciarControle();
    }

    this_thread::sleep_for(chrono::seconds(tempoExecucaoSeg));

    for (auto& controlador : controladores) {
        controlador->pararControle();
    }

    cout << "Simulacao concluida." << endl;

    return 0;
}