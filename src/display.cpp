#include "display.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <atomic>
#include <unordered_map>
#include <chrono>

using namespace std;

std::mutex Display::guiMutex;

std::unordered_map<std::string, cv::Mat> Display::frameBuffers;
std::unordered_map<std::string, std::unique_ptr<std::mutex>> Display::frameMutexes;
std::mutex Display::mapMutex;

Display::Display(string caminhoImagemBase, string nomeJanela)
    : caminhoImagemBase(std::move(caminhoImagemBase)), nomeJanela(std::move(nomeJanela)) {
    Display::registerWindow(this->nomeJanela);
}

cv::Mat Display::gerarImagem(string consumo) {
    cv::Mat imagemBase = cv::imread(this->caminhoImagemBase);

    if (imagemBase.empty()) {
        cerr << "ERRO: Nao foi possivel carregar a imagem" << endl;
        return cv::Mat();
    }

    int qntDigitos = 6;
    string valorDisplay = string(qntDigitos - consumo.size(), '0') + consumo;

    cv::Mat frame = imagemBase.clone();

    int pos_x = 408, pos_y = 390;

    int tipoFonte = cv::FONT_HERSHEY_PLAIN;
    double escalaFonte = 2.8;
    cv::Scalar corTexto(0, 0, 0);
    int espessura = 4;

    int cont = 0;

    for (auto digito : valorDisplay) {
        if (cont >= 4)
            corTexto = cv::Scalar(0, 0, 255);

        cv::Point posicaoTexto(pos_x, pos_y);
        cv::putText(frame, string(1, digito), posicaoTexto, tipoFonte, escalaFonte, corTexto, espessura, cv::LINE_AA);
        pos_x += 38;
        cont++;
    }

    return frame;
}

void Display::salvarImagemJpeg(cv::Mat& frame, string caminho) {
    if (!cv::imwrite(caminho, frame)) {
        cerr << "ERRO: nao foi possivel salvar a imagem em " << caminho << endl;
    }
}

void Display::fecharJanela() {
    Display::unregisterWindow(this->nomeJanela);
}

void Display::registerWindow(const std::string& nomeJanela) {
    std::lock_guard<std::mutex> lock(mapMutex);
    if (frameBuffers.find(nomeJanela) == frameBuffers.end()) {
        frameBuffers.emplace(nomeJanela, cv::Mat());
        // default mutex for this buffer
    frameMutexes.emplace(nomeJanela, std::make_unique<std::mutex>());
        // create native window under gui mutex
        {
            std::lock_guard<std::mutex> gLock(guiMutex);
            cv::namedWindow(nomeJanela, cv::WINDOW_NORMAL);
            // set a reasonable initial window size, but do not force position every frame
            cv::resizeWindow(nomeJanela, 800, 700);
        }
    }
}

void Display::unregisterWindow(const std::string& nomeJanela) {
    {
        std::lock_guard<std::mutex> lock(mapMutex);
        auto it = frameBuffers.find(nomeJanela);
        if (it != frameBuffers.end()) {
            frameBuffers.erase(it);
            frameMutexes.erase(nomeJanela);
        }
    }
    std::lock_guard<std::mutex> gLock(guiMutex);
    cv::destroyWindow(nomeJanela);
}

void Display::pushFrame(const std::string& nomeJanela, const cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(mapMutex);
    auto it = frameBuffers.find(nomeJanela);
    if (it == frameBuffers.end())
        return;

    // copy into buffer under its mutex
    {
        auto mit = frameMutexes.find(nomeJanela);
        if (mit != frameMutexes.end()) {
            std::lock_guard<std::mutex> fl(*mit->second);
            it->second = frame.clone();
        }
    }

    // no manager thread anymore; nothing to notify
}

void Display::processEvents(int waitMs) {
    // copy keys under mapMutex
    std::vector<std::string> keys;
    {
        std::lock_guard<std::mutex> lock(mapMutex);
        keys.reserve(frameBuffers.size());
        for (const auto& kv : frameBuffers)
            keys.push_back(kv.first);
    }

    for (const auto& name : keys) {
        cv::Mat frameCopy;
        // copy latest frame under its mutex
        auto mit = frameMutexes.find(name);
        if (mit != frameMutexes.end() && mit->second) {
            std::lock_guard<std::mutex> fl(*mit->second);
            auto fit = frameBuffers.find(name);
            if (fit != frameBuffers.end())
                frameCopy = fit->second.clone();
        }

        if (!frameCopy.empty()) {
            std::lock_guard<std::mutex> gLock(guiMutex);
            cv::imshow(name, frameCopy);
        }
    }

    // process GUI events
    cv::waitKey(waitMs);
}