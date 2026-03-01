#pragma once
#include <vector>
#include <mutex>

class CircularBuffer {
private:
    std::vector<double> buffer;
    size_t head;
    size_t count;
    size_t capacity;
    std::mutex mtx; // Thread safety for background polling

public:
    explicit CircularBuffer(size_t size) : capacity(size), head(0), count(0), buffer(size, 0.0) {}

    void add(double value) {
        std::lock_guard<std::mutex> lock(mtx);
        buffer[head] = value;
        head = (head + 1) % capacity;
        if (count < capacity) {
            count++;
        }
    }

    double getAverage() {
        std::lock_guard<std::mutex> lock(mtx);
        if (count == 0) return 0.0;
        
        double sum = 0.0;
        for (size_t i = 0; i < count; ++i) {
            sum += buffer[i];
        }
        return sum / count;
    }
    
    void clear() {
        std::lock_guard<std::mutex> lock(mtx);
        head = 0;
        count = 0;
    }
};