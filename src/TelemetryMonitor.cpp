#include "TelemetryMonitor.h"
#include <iostream>

// Link the PDH library
#pragma comment(lib, "pdh.lib")

TelemetryMonitor::TelemetryMonitor() 
    : pdhInitialized(false), 
      rng(std::random_device{}()), 
      cpuTempDist(35.0, 85.0), // Simulate temps between 35C and 85C
      gpuTempDist(40.0, 75.0) 
{
    initPDH();
}

TelemetryMonitor::~TelemetryMonitor() {
    if (pdhInitialized) {
        PdhCloseQuery(cpuQuery);
    }
}

void TelemetryMonitor::initPDH() {
    // Open a PDH query to track total CPU time
    if (PdhOpenQuery(NULL, NULL, &cpuQuery) == ERROR_SUCCESS) {
        if (PdhAddEnglishCounter(cpuQuery, "\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal) == ERROR_SUCCESS) {
            PdhCollectQueryData(cpuQuery);
            pdhInitialized = true;
        }
    }
}

double TelemetryMonitor::getCpuLoad() {
    if (!pdhInitialized) return 0.0;
    
    PDH_FMT_COUNTERVALUE counterVal;
    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    
    return counterVal.doubleValue;
}

double TelemetryMonitor::getCpuTemperature() {
    // Fallback simulated reading for WMI query constraints
    return cpuTempDist(rng); 
}

double TelemetryMonitor::getGpuTemperature() {
    // Fallback simulated reading for WMI query constraints
    return gpuTempDist(rng); 
}