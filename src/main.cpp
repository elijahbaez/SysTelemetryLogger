#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <csignal>
#include "TelemetryMonitor.h"
#include "CircularBuffer.h"
#include "DatabaseManager.h"

// Atomic flag to handle safe thread termination
std::atomic<bool> keepRunning(true);

// Capture Ctrl+C to cleanly stop the while loop and close the DB
void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received. Shutting down gracefully...\n";
    keepRunning = false;
}

int main() {
    // Register signal handler
    std::signal(SIGINT, signalHandler);

    std::cout << "Starting C++ System Telemetry Logger...\n";
    std::cout << "Target overhead: <2% CPU | Polling: 1s. Press Ctrl+C to exit.\n\n";

    TelemetryMonitor monitor;
    DatabaseManager dbManager("telemetry_metrics.db");

    // Initialize circular buffers for a 5-second rolling average window
    CircularBuffer cpuTempBuf(5);
    CircularBuffer gpuTempBuf(5);

    while (keepRunning) {
        // 1. Capture sensor data (Windows API via TelemetryMonitor)
        double cpuLoad = monitor.getCpuLoad();
        double cpuTemp = monitor.getCpuTemperature();
        double gpuTemp = monitor.getGpuTemperature();

        // 2. Feed raw temps into circular buffer
        cpuTempBuf.add(cpuTemp);
        gpuTempBuf.add(gpuTemp);

        // 3. Retrieve rolling averages
        double avgCpuTemp = cpuTempBuf.getAverage();
        double avgGpuTemp = gpuTempBuf.getAverage();

        // 4. Persist to SQLite
        dbManager.insertTelemetry(cpuLoad, avgCpuTemp, avgGpuTemp);

        // 5. Lightweight console output
        printf("[LOG] CPU Load: %5.2f%% | CPU Temp (5s Avg): %5.2fC | GPU Temp (5s Avg): %5.2fC\n", 
               cpuLoad, avgCpuTemp, avgGpuTemp);

        // Sleep for exactly 1 second (1-second polling interval)
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Background utility stopped successfully.\n";
    return 0;
}