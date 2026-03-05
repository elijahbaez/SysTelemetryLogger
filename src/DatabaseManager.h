#pragma once
#include <string>

// Forward declaration for sqlite3
struct sqlite3;

class DatabaseManager {
private:
    sqlite3* db;
    bool executeQuery(const std::string& query);

public:
    DatabaseManager(const std::string& dbName);
    ~DatabaseManager();
    
    // Inserts the hardware telemetry row into the local database
    bool insertTelemetry(double cpuLoad, double avgCpuTemp, double avgGpuTemp);
};