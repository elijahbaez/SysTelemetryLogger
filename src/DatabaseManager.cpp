#include "DatabaseManager.h"
#include "../include/sqlite3.h"
#include <iostream>

DatabaseManager::DatabaseManager(const std::string& dbName) : db(nullptr) {
    // Open (or create) the SQLite database file
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << "\n";
    } else {
        // Create the table if it doesn't exist. 
        // We use CURRENT_TIMESTAMP to automatically log the time of each entry.
        std::string createTable = 
            "CREATE TABLE IF NOT EXISTS telemetry ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
            "cpu_load REAL, "
            "cpu_temp_avg REAL, "
            "gpu_temp_avg REAL);";
        executeQuery(createTable);
    }
}

DatabaseManager::~DatabaseManager() {
    if (db) {
        sqlite3_close(db);
    }
}

bool DatabaseManager::executeQuery(const std::string& query) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL Error: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool DatabaseManager::insertTelemetry(double cpuLoad, double avgCpuTemp, double avgGpuTemp) {
    // Construct the SQL INSERT statement
    std::string query = "INSERT INTO telemetry (cpu_load, cpu_temp_avg, gpu_temp_avg) VALUES (" +
                        std::to_string(cpuLoad) + ", " + 
                        std::to_string(avgCpuTemp) + ", " + 
                        std::to_string(avgGpuTemp) + ");";
    return executeQuery(query);
}