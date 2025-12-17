#include "display.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <atomic>
#include <unordered_map>
#include <chrono>
#include <cstdlib> // Para rand()
#include <ctime>   // Para time()
#include <map>     // Para guardar os contadores de cada janela

using namespace std;

// --- VARIÁVEIS GLOBAIS DESTE ARQUIVO (OCULTAS) ---
// Mapa para guardar o consumo atual de cada janela (hidrometro)
// Assim, cada hidrômetro tem sua própria contagem que sobe independentemente.
static std::map<std::string, long long> contadoresSimulados;
static bool sementeAleatoriaIniciada = false;

std::mutex Display::guiMutex;
std::unordered_map<std::string, cv::Mat> Display::frameBuffers;
std::unordered_map<std::string, std::unique_ptr<std::mutex>> Display::frameMutexes;
std::mutex Display::mapMutex;

Display::Display(string caminhoImagemBase, string nomeJanela)
    : caminhoImagemBase(std::move(caminhoImagemBase)), nomeJanela(std::move(nomeJanela)) {
    Display::registerWindow(this->nomeJanela);
    
    // Inicia a semente aleatória uma única vez
    if (!sementeAleatoriaIniciada) {
        srand(static_cast<unsigned int>(time(NULL)));
        sementeAleatoriaIniciada = true;
    }
    
    // Se este hidrômetro ainda não tem contador, inicia com um valor base (ex: 150)
    if (contadoresSimulados.find(this->nomeJanela) == contadoresSimulados.end()) {
        contadoresSimulados[this->nomeJanela] = 150; 
    }
}

cv::Mat Display::gerarImagem(string consumo) {
    // --- LÓGICA DE SIMULAÇÃO (IGNORA O PARAMETRO 'consumo') ---
    
    // 1. Incrementa o contador deste hidrômetro específico
    // Soma um valor aleatório pequeno (1 a 3) para o gráfico subir suave
    int incremento = (rand() % 3) + 1; 
    contadoresSimulados[this->nomeJanela] += incremento;
    
    long long valorAtual = contadoresSimulados[this->nomeJanela];

    // 2. Formata para o display (ex: 000153)
    int qntDigitos = 6;
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(qntDigitos) << valorAtual;
    string valorDisplay = ss.str();
    // ---------------------------------------------------------

    cv::Mat imagemBase = cv::imread(this->caminhoImagemBase);

    if (imagemBase.empty()) {
        cerr << "ERRO: Nao foi possivel carregar a imagem" << endl;
        return cv::Mat();
    }

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
    // Recupera o valor atual deste hidrômetro
    long long valorAtual = contadoresSimulados[this->nomeJanela];

    // 1. Descobre em qual pasta salvar (extrai do caminho original)
    string pasta = "";
    size_t barraPos = caminho.find_last_of("/\\");
    if (barraPos != string::npos) {
        pasta = caminho.substr(0, barraPos + 1);
    }

    // 2. Descobre a extensão (.png, .jpg)
    string extensao = ".png"; 
    size_t pontoPos = caminho.find_last_of('.');
    if (pontoPos != string::npos) {
        extensao = caminho.substr(pontoPos);
    }

    // 3. TRUQUE: Usa o Nome da Janela (ex: "sha_1") como nome do arquivo
    // Isso garante que o ID seja sempre o mesmo, não importa o frame
    string nomeFixo = this->nomeJanela;
    
    // Substitui espaços por underline para evitar problemas
    for(auto &c : nomeFixo) {
        if(c == ' ') c = '_';
    }

    // 4. Monta o novo nome: pasta/sha_1_VALOR_150.png
    // Note que ignoramos totalmente o nome "frame_01" que veio no 'caminho'
    string novoCaminho = pasta + nomeFixo + "_VALOR_" + to_string(valorAtual) + extensao;

    if (!cv::imwrite(novoCaminho, frame)) {
        cerr << "ERRO: nao foi possivel salvar a imagem em " << novoCaminho << endl;
    }
}

void Display::fecharJanela() {
    Display::unregisterWindow(this->nomeJanela);
}

void Display::registerWindow(const std::string& nomeJanela) {
    std::lock_guard<std::mutex> lock(mapMutex);
    if (frameBuffers.find(nomeJanela) == frameBuffers.end()) {
        frameBuffers.emplace(nomeJanela, cv::Mat());
        frameMutexes.emplace(nomeJanela, std::make_unique<std::mutex>());
        {
            std::lock_guard<std::mutex> gLock(guiMutex);
            cv::namedWindow(nomeJanela, cv::WINDOW_NORMAL);
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

    {
        auto mit = frameMutexes.find(nomeJanela);
        if (mit != frameMutexes.end()) {
            std::lock_guard<std::mutex> fl(*mit->second);
            it->second = frame.clone();
        }
    }
}

void Display::processEvents(int waitMs) {
    std::vector<std::string> keys;
    {
        std::lock_guard<std::mutex> lock(mapMutex);
        keys.reserve(frameBuffers.size());
        for (const auto& kv : frameBuffers)
            keys.push_back(kv.first);
    }

    for (const auto& name : keys) {
        cv::Mat frameCopy;
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
    cv::waitKey(waitMs);
}