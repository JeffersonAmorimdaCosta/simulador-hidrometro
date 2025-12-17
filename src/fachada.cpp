#include "fachada.hpp"
#include "cano.hpp"
#include "hidrometro.hpp"
#include "display.hpp"
#include "controlador.hpp"
#include <iostream>
#include <sstream>

using namespace std;

struct FachadaSHA::Impl {
    std::mutex mtx;
    int nextId{1};
    struct Instance {
        unique_ptr<Cano> entrada;
        unique_ptr<Cano> saida;
        unique_ptr<Hidrometro> hidrometro;
        unique_ptr<Display> display;
        unique_ptr<Controlador> controlador;
        std::string nomeJanela;
    };

    map<int, Instance> instancias;

    int intervaloImagemMs{70};
    int intervaloVazao{50};
    int volumeMaximo{999};
    float perdaAr{0.1f};
};

FachadaSHA::FachadaSHA() : impl(make_unique<Impl>()) {
}

FachadaSHA::~FachadaSHA() {
    for (auto &kv : impl->instancias) {
        if (kv.second.controlador) kv.second.controlador->pararControle();
        if (kv.second.display) kv.second.display->fecharJanela();
    }
}

FachadaSHA& FachadaSHA::instance() {
    static FachadaSHA s_instance;
    return s_instance;
}

void FachadaSHA::configSimuladorSHA(int intervaloImagemMs, int intervaloVazao, int volumeMaximo, float perdaAr) {
    std::lock_guard<std::mutex> lock(impl->mtx);
    impl->intervaloImagemMs = intervaloImagemMs;
    impl->intervaloVazao = intervaloVazao;
    impl->volumeMaximo = volumeMaximo;
    impl->perdaAr = perdaAr;
}

int FachadaSHA::criaSHA(const std::string& nome, float bitolaEntrada, float bitolaSaida) {
    std::lock_guard<std::mutex> lock(impl->mtx);
    int id = impl->nextId++;

    Impl::Instance inst;
    inst.entrada = make_unique<Cano>(bitolaEntrada, impl->intervaloVazao);
    inst.saida = make_unique<Cano>(bitolaSaida, impl->intervaloVazao);
    inst.hidrometro = make_unique<Hidrometro>(*inst.entrada, *inst.saida, impl->volumeMaximo, impl->perdaAr);

    string nomeJanela = nome + "-" + to_string(id);

    // --- CORREÇÃO AQUI ---
    // Usamos .parent_path() para sair da pasta 'build'
    // E mudamos de 'medicoes' para 'images' para o painel achar fácil
    filesystem::path dirInstancia = filesystem::current_path().parent_path() / "images" / ("sha_" + to_string(id));
    
    // Cria a pasta se ela não existir (para evitar erros)
    filesystem::create_directories(dirInstancia);
    // ---------------------

    inst.display = make_unique<Display>("../images/base/imagem_base_hidrometro.png", nomeJanela);
    inst.controlador = make_unique<Controlador>(impl->intervaloImagemMs, *inst.hidrometro, *inst.display, dirInstancia);
    inst.nomeJanela = nomeJanela;

    inst.controlador->iniciarControle();

    impl->instancias.emplace(id, std::move(inst));

    return id;
}

bool FachadaSHA::finalizaSHA(int id) {
    std::lock_guard<std::mutex> lock(impl->mtx);
    auto it = impl->instancias.find(id);
    if (it == impl->instancias.end()) return false;

    auto &inst = it->second;
    
    // 1. Para as threads e fecha janela
    if (inst.controlador) {
        inst.controlador->pararControle();
    }
    if (inst.display) inst.display->fecharJanela();

    // 2. --- NOVO: Apaga a pasta de imagens ---
    try {
        // Recalcula o caminho da pasta (mesma lógica do criaSHA)
        std::filesystem::path dirInstancia = std::filesystem::current_path().parent_path() / "images" / ("sha_" + std::to_string(id));
        
        // Verifica se existe e apaga tudo dentro
        if (std::filesystem::exists(dirInstancia)) {
            std::error_code ec;
            std::filesystem::remove_all(dirInstancia, ec);
            
            if (ec) {
                std::cerr << "[Aviso] Nao foi possivel apagar a pasta: " << dirInstancia << " (" << ec.message() << ")" << std::endl;
            } else {
                std::cout << "[Info] Pasta apagada: " << dirInstancia.filename().string() << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[Erro] Falha ao tentar apagar pasta: " << e.what() << std::endl;
    }
    // -----------------------------------------

    impl->instancias.erase(it);
    return true;
}

bool FachadaSHA::modificaVazaoSHA(int id, float novaVazao) {
    std::lock_guard<std::mutex> lock(impl->mtx);
    auto it = impl->instancias.find(id);
    if (it == impl->instancias.end()) return false;

    auto &inst = it->second;
    if (inst.hidrometro) {
        inst.hidrometro->getEntrada().setVazao(novaVazao);
        return true;
    }
    return false;
}

bool FachadaSHA::habilitaGeracaoImagemSHA(int id, bool habilitar) {
    std::lock_guard<std::mutex> lock(impl->mtx);
    auto it = impl->instancias.find(id);
    if (it == impl->instancias.end()) return false;

    auto &inst = it->second;
    if (inst.controlador) {
        inst.controlador->setSalvarImagens(habilitar);
        return true;
    }
    return false;
}

vector<int> FachadaSHA::listarInstancias() {
    std::lock_guard<std::mutex> lock(impl->mtx);
    vector<int> ids;
    ids.reserve(impl->instancias.size());
    for (auto &kv : impl->instancias) ids.push_back(kv.first);
    return ids;
}

bool FachadaSHA::obterInfoInstancia(int id, InstanceInfo &out) {
    std::lock_guard<std::mutex> lock(impl->mtx);
    auto it = impl->instancias.find(id);
    if (it == impl->instancias.end()) return false;
    auto &inst = it->second;
    out.id = id;
    out.nomeJanela = inst.nomeJanela;
    if (inst.entrada) out.vazaoEntrada = inst.entrada->getVazao();
    else out.vazaoEntrada = 0.0f;
    if (inst.controlador) out.salvarImagens = inst.controlador->getSalvarImagens();
    else out.salvarImagens = false;
    return true;
}