#pragma once
#include <windows.h>
#include <pdh.h>
#include <random>

class TelemetryMonitor {
private:
    PDH_HQUERY cpuQuery;
    PDH_HCOUNTER cpuTotal;
    bool pdhInitialized;

    // Fallback RNG for thermals if WMI is blocked by motherboard/ring-0 constraints
    std::mt19937 rng;
    std::uniform_real_distribution<double> cpuTempDist;
    std::uniform_real_distribution<double> gpuTempDist;

public:
    TelemetryMonitor();
    ~TelemetryMonitor();

    void initPDH();
    double getCpuLoad();
    double getCpuTemperature(); 
    double getGpuTemperature(); 
};