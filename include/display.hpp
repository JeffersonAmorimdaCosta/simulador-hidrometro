#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include "hidrometro.hpp"
#include <mutex>
#include <unordered_map>
#include <thread>
#include <atomic>
#include <condition_variable>

using namespace std;

class Display {
    private:
        string caminhoImagemBase, nomeJanela;
        static std::mutex guiMutex;

        // Display manager (single thread) data structures
    static std::unordered_map<std::string, cv::Mat> frameBuffers;
    static std::unordered_map<std::string, std::unique_ptr<std::mutex>> frameMutexes;
    static std::mutex mapMutex;

    public:
    Display(string caminhoImagemBase, string nomeJanela);

    cv::Mat gerarImagem(string consumo);
    void salvarImagemJpeg(cv::Mat& frame, string caminho);
    void fecharJanela();

    // manager control
    // Process display events (must be called from the main thread on Windows)
    static void processEvents(int waitMs = 5);
    static void registerWindow(const std::string& nomeJanela);
    static void unregisterWindow(const std::string& nomeJanela);
    static void pushFrame(const std::string& nomeJanela, const cv::Mat& frame);

    string getNomeJanela() const { return nomeJanela; }
};