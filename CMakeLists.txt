cmake_minimum_required(VERSION 3.10)
project(SysTelemetryLogger VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

# Include SQLite
include_directories(include)

# Add source files
add_executable(SysTelemetryLogger 
    src/main.cpp 
    src/TelemetryMonitor.cpp
    src/DatabaseManager.cpp 
    include/sqlite3.c
)

# Link Windows libraries required for system metrics
target_link_libraries(SysTelemetryLogger pdh wbemuuid)

# Apply aggressive compiler optimizations for Release builds to maintain <2% CPU overhead
target_compile_options(SysTelemetryLogger PRIVATE "$<$<CONFIG:Release>:/O2>")