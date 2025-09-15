#include "Display.hpp"
#include <cairomm/cairomm.h>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

void Display::gerarImagem(string consumo) {
    int qnt_digitos = 6;
    
    string sequenciaStr = to_string(this->sequencia);
    this->avancarSequencia();

    string caminhoImagemSaida = "../images/medicoes_202311250016/" + sequenciaStr + ".png";

    string valorDisplay = string(qnt_digitos - consumo.size(), '0') + consumo;

    try {
        Cairo::RefPtr<Cairo::ImageSurface> surface = 
            Cairo::ImageSurface::create_from_png(this->caminhoImagemBase);
        Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);

        cr->set_source_rgb(0.0, 0.0, 0.0);

        cr->select_font_face("Sans",
            Cairo::ToyFontFace::Slant::NORMAL,
            Cairo::ToyFontFace::Weight::BOLD);

        cr->set_font_size(45.0);

        int y = 388, xInicial = 410, incremento = 38, cont = 0;

        for (auto digito : valorDisplay) {
            if (cont < 4) {
                cr->set_source_rgb(0.0, 0.0, 0.0);
            } else {
                cr->set_source_rgb(1.0, 0.0, 0.0);
            }
            
            cr->move_to(xInicial + (cont * incremento), y);
            string digitoStr(1, digito);
            cr->show_text(digitoStr);
            cont++;
        }

        surface->write_to_png(caminhoImagemSaida);

    } catch (const exception& ex) {
        cerr << "Erro: " << ex.what() << endl;
    }
}
