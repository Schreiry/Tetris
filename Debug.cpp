// Debug.cpp
#include "Debug.h"

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <unistd.h>
#include <fstream>
#include <sstream>
#endif

// Инициализация статических переменных
float Debug::currentFPS = 0.0f;
std::chrono::steady_clock::time_point Debug::lastUpdate = std::chrono::steady_clock::now();

void Debug::Log(const std::string& message) {
    // Получаем текущее время и форматируем его
    auto now = std::chrono::system_clock::now();
    std::time_t timeT = std::chrono::system_clock::to_time_t(now);
    std::cout << "[" << std::put_time(std::localtime(&timeT), "%H:%M:%S") << "] " << message << std::endl;
}

void Debug::Update() {
    // Здесь можно обновлять метрики, если потребуется (например, считать FPS на основе времени между вызовами)
    // Для простоты можно оставить пустой, а значение FPS обновлять через SetFPS()
}

void Debug::SetFPS(float fps) {
    currentFPS = fps;
}

size_t Debug::GetMemoryUsage() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize; // в байтах
    }
    return 0;
#else
    // Для Linux: читаем значение VmRSS из /proc/self/status
    std::ifstream statStream("/proc/self/status");
    std::string line;
    while (std::getline(statStream, line)) {
        if (line.substr(0, 6) == "VmRSS:") {
            std::istringstream iss(line);
            std::string key;
            size_t memory;
            std::string unit;
            iss >> key >> memory >> unit;
            return memory * 1024; // значение в байтах
        }
    }
    return 0;
#endif
}

float Debug::GetCPUUsage() {
    // Получение загрузки процессора является сложной задачей и требует платформенно-специфичного кода.
    // В данном примере возвращаем 0.0f. Для Windows можно использовать GetProcessTimes, для Linux – /proc/stat.
    return 0.0f;
}

void Debug::PrintPerformanceInfo() {
    size_t memUsage = GetMemoryUsage();
    float cpuUsage = GetCPUUsage();

    // Красивый и презентабельный вывод в консоль
    std::cout << "----- Performance Info -----" << std::endl;
    std::cout << "FPS           : " << currentFPS << std::endl;
    std::cout << "Memory Usage  : " << memUsage / 1024 << " KB" << std::endl;
    std::cout << "CPU Usage     : " << cpuUsage << " %" << std::endl;
    std::cout << "-----------------------------" << std::endl;
}
