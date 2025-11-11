#pragma once

#include <memory>
#include <map>
#include <filesystem>
#include <string>
#include <vector>

class FachadaSHA {
private:
    struct Impl;

    std::unique_ptr<Impl> impl;
    FachadaSHA();

    FachadaSHA(const FachadaSHA&) = delete;
    FachadaSHA& operator=(const FachadaSHA&) = delete;
    FachadaSHA(FachadaSHA&&) = delete;
    FachadaSHA& operator=(FachadaSHA&&) = delete;

public:
    ~FachadaSHA();
    static FachadaSHA& instance();

    void configSimuladorSHA(int intervaloImagemMs = 70, int intervaloVazao = 50, int volumeMaximo = 999, float perdaAr = 0.1f);

    int criaSHA(const std::string& nome = "SHA", float bitolaEntrada = 1.5f, float bitolaSaida = 2.0f);

    bool finalizaSHA(int id);

    bool modificaVazaoSHA(int id, float novaVazao);

    bool habilitaGeracaoImagemSHA(int id, bool habilitar);

    std::vector<int> listarInstancias();

    struct InstanceInfo {
        int id;
        std::string nomeJanela;
        float vazaoEntrada;
        bool salvarImagens;
    };

    bool obterInfoInstancia(int id, InstanceInfo &out);
};
