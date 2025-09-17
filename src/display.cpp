#include "Display.hpp"
#include <cairomm/cairomm.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <opencv2/opencv.hpp>

using namespace std;

void Display::gerarImagem(string consumo, string nomeImagem) {
    int qnt_digitos = 6;
    
    string caminhoImagemSaida = nomeImagem;

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

void Display::pngParaJpeg(const string caminhoImagem) {
    string saida;
    if (caminhoImagem.size() >= 4 && caminhoImagem.substr(caminhoImagem.size() - 4) == ".png")
        saida = caminhoImagem.substr(0, caminhoImagem.size() - 4) + ".jpeg";
    else {
        cerr << "O arquivo não é .png" << endl;
        return;
    }

    cv::Mat img = cv::imread(caminhoImagem, cv::IMREAD_UNCHANGED);
    if (img.empty()) {
        cerr << "Erro: não foi possível abrir " << caminhoImagem << endl;
        return;
    }

    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(95);  // qualidade alta (95%)

    if (!cv::imwrite(saida, img, params)) {
        cerr << "Erro: não foi possível salvar " << saida << endl;
        return;
    }
}
