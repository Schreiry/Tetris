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

//<><> <><> this module does not work yet <><> <><>

// Initialization of static variables
float Debug::currentFPS = 0.0f;
std::chrono::steady_clock::time_point Debug::lastUpdate = std::chrono::steady_clock::now();

void Debug::Log(const std::string& message) {
    // Get the current time and format it
    auto now = std::chrono::system_clock::now();
    std::time_t timeT = std::chrono::system_clock::to_time_t(now);
    std::cout << "[" << std::put_time(std::localtime(&timeT), "%H:%M:%S") << "] " << message << std::endl;
}

void Debug::Update() {
    // Çäåñü ìîæíî îáíîâëÿòü ìåòðèêè, åñëè ïîòðåáóåòñÿ (íàïðèìåð, ñ÷èòàòü FPS íà îñíîâå âðåìåíè ìåæäó âûçîâàìè)
    // Äëÿ ïðîñòîòû ìîæíî îñòàâèòü ïóñòîé, à çíà÷åíèå FPS îáíîâëÿòü ÷åðåç SetFPS()
}

void Debug::SetFPS(float fps) {
    currentFPS = fps;
}

size_t Debug::GetMemoryUsage() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize; // â áàéòàõ
    }
    return 0;
#else
    // Äëÿ Linux: ÷èòàåì çíà÷åíèå VmRSS èç /proc/self/status
    std::ifstream statStream("/proc/self/status");
    std::string line;
    while (std::getline(statStream, line)) {
        if (line.substr(0, 6) == "VmRSS:") {
            std::istringstream iss(line);
            std::string key;
            size_t memory;
            std::string unit;
            iss >> key >> memory >> unit;
            return memory * 1024; // çíà÷åíèå â áàéòàõ
        }
    }
    return 0;
#endif
}

float Debug::GetCPUUsage() {
    // Ïîëó÷åíèå çàãðóçêè ïðîöåññîðà ÿâëÿåòñÿ ñëîæíîé çàäà÷åé è òðåáóåò ïëàòôîðìåííî-ñïåöèôè÷íîãî êîäà.
    // Â äàííîì ïðèìåðå âîçâðàùàåì 0.0f. Äëÿ Windows ìîæíî èñïîëüçîâàòü GetProcessTimes, äëÿ Linux – /proc/stat.
    return 0.0f;
}

void Debug::PrintPerformanceInfo() {
    size_t memUsage = GetMemoryUsage();
    float cpuUsage = GetCPUUsage();

    // Êðàñèâûé è ïðåçåíòàáåëüíûé âûâîä â êîíñîëü
    std::cout << "----- Performance Info -----" << std::endl;
    std::cout << "FPS           : " << currentFPS << std::endl;
    std::cout << "Memory Usage  : " << memUsage / 1024 << " KB" << std::endl;
    std::cout << "CPU Usage     : " << cpuUsage << " %" << std::endl;
    std::cout << "-----------------------------" << std::endl;
}
