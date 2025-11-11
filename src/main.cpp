#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#include "fachada.hpp"
#include "display.hpp"
#include "cli.hpp"

using namespace std;

int main(int argc, char** argv) {
    cout << "SHA Simulator CLI (type 'help' for commands)" << endl;

    // CLI is handled by CLIClient; it owns its stdin reader. No global input thread here.

    auto& fachada = FachadaSHA::instance();

    // run CLI loop (the only supported interface in this delivery)
    CLIClient::runCLI(fachada);

    // CLI loop handled in CLIClient::runCLI(fachada) above.

    // CLIClient handles its own threads; nothing to join here.

    cout << "Saindo..." << endl;
    return 0;
}