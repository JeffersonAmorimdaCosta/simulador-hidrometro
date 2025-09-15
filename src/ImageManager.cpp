#include "ImageManager.hpp"
#include <string>
#include <iostream>

#if defined(_WIN32)
#  define NOMINMAX
#  define WIN32_LEAN_AND_MEAN
#  define byte win_byte_override
#  include <windows.h>
#  undef byte
#endif

Handle ImageManager::abrirImagem(const std::string& caminho) {
    STARTUPINFOA si{};
    PROCESS_INFORMATION pi{};

    si.cb = sizeof(si);

    std::string comando = "rundll32.exe \"C:\\Program Files\\Windows Photo Viewer\\PhotoViewer.dll\", ImageView_Fullscreen " + caminho;


    if (!CreateProcessA(
            NULL,
            comando.data(),
            NULL, NULL,
            FALSE,
            0,
            NULL, NULL,
            &si,
            &pi
        )) {
        std::cerr << "Erro ao abrir arquivo\n";
    }

    return {pi.hProcess, pi.hThread}; // guarda PID e handles
}

void ImageManager::fecharImagem(Handle& pi) {
    TerminateProcess(pi.hProcess, 0);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
