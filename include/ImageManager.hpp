#pragma once

#include <iostream>

using namespace std;

#ifdef _WIN32
struct Handle {
    void* hProcess{};
    void* hThread{};
};
#endif

class ImageManager {
    public:
        ImageManager() = default;

        Handle abrirImagem(const string& caminho);

        void fecharImagem(Handle& pi);

};