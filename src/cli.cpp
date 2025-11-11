#include "cli.hpp"
#include "display.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <sstream>
#include <string>

using namespace std;

void CLIClient::runCLI(FachadaSHA &fachada) {
    queue<string> cmdQueue;
    mutex qMutex;
    condition_variable_any qCv;
    atomic<bool> done{false};

    thread inputThread([&]() {
        string line;
        while (true) {
            if (!std::getline(cin, line)) break;
            if (!line.empty() && line.back() == '\r') line.pop_back();
            {
                lock_guard<mutex> lk(qMutex);
                cmdQueue.push(line);
            }
            qCv.notify_all();
            if (line == "exit") break;
        }
        done = true;
        qCv.notify_all();
    });

    bool running = true;
    while (running && !done.load()) {
        Display::processEvents(5);

        string cmd;
        {
            lock_guard<mutex> lk(qMutex);
            if (!cmdQueue.empty()) { cmd = cmdQueue.front(); cmdQueue.pop(); }
        }

        if (!cmd.empty()) {
            istringstream iss(cmd);
            string op; iss >> op;
            if (op == "help") {
                cout << "Commands:\n";
                cout << "  config <intervaloImagemMs> <intervaloVazao> <volumeMaximo> <perdaAr>\n";
                cout << "  cria <nome> [bitolaEntrada] [bitolaSaida]\n";
                cout << "  finaliza <id>\n";
                cout << "  modificaVazao <id> <novaVazao>\n";
                cout << "  habilitaImagem <id> <0|1>\n";
                cout << "  listar\n";
                cout << "  exit\n";
            } else if (op == "config") {
                int intervaloImagemMs, intervaloVazao, volumeMaximo; float perdaAr;
                if (iss >> intervaloImagemMs >> intervaloVazao >> volumeMaximo >> perdaAr) {
                    fachada.configSimuladorSHA(intervaloImagemMs, intervaloVazao, volumeMaximo, perdaAr);
                    cout << "Config atualizada.\n";
                } else cout << "usage: config <intervaloImagemMs> <intervaloVazao> <volumeMaximo> <perdaAr>\n";
            } else if (op == "cria") {
                string nome; float be=1.5f, bs=2.0f;
                if (iss >> nome) {
                    iss >> be >> bs; // optional
                } else nome = "SHA";
                int id = fachada.criaSHA(nome, be, bs);
                cout << "Criada instancia id=" << id << "\n";
            } else if (op == "finaliza") {
                int id; if (iss >> id) {
                    if (fachada.finalizaSHA(id)) cout << "Finalizada id="<<id<<"\n";
                    else cout << "Instancia nao encontrada: "<<id<<"\n";
                } else cout << "usage: finaliza <id>\n";
            } else if (op == "modificaVazao") {
                int id; float v; if (iss >> id >> v) {
                    if (fachada.modificaVazaoSHA(id, v)) cout << "Vazao atualizada\n";
                    else cout << "Instancia nao encontrada\n";
                } else cout << "usage: modificaVazao <id> <novaVazao>\n";
            } else if (op == "habilitaImagem") {
                int id; int flag; if (iss >> id >> flag) {
                    if (fachada.habilitaGeracaoImagemSHA(id, flag!=0)) cout << "Imagem flag set\n";
                    else cout << "Instancia nao encontrada\n";
                } else cout << "usage: habilitaImagem <id> <0|1>\n";
            } else if (op == "listar") {
                auto ids = fachada.listarInstancias();
                cout << "Instancias: ";
                for (auto i : ids) cout << i << " "; cout << "\n";
            } else if (op == "exit") {
                // finalize all and exit
                auto ids = fachada.listarInstancias();
                for (auto id : ids) fachada.finalizaSHA(id);
                running = false;
                break;
            } else {
                cout << "Comando desconhecido. Digite 'help'.\n";
            }
        }

        this_thread::sleep_for(chrono::milliseconds(5));
    }

    if (inputThread.joinable()) inputThread.join();
}
