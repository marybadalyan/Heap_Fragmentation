#include <cstdlib>
#include <vector>
#include <iostream>
#include <chrono>
#include <malloc.h>
#include <cstring>   
constexpr int allocCount = 1000;
constexpr int blockSize = 1024;
constexpr size_t largeSize = blockSize * allocCount / 2;
constexpr int trials = 10;



double time_large_alloc(bool touch_memory = true) {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    void* ptr = malloc(largeSize);
    auto end = high_resolution_clock::now();

    if (ptr && touch_memory)
        memset(ptr, 0, largeSize); 

    if (ptr) free(ptr);
    return duration<double, std::micro>(end - start).count();
}

int main() {

    std::vector<void*> blocks(allocCount);

    double beforeTotal = 0;
    for (int i = 0; i < trials; ++i)
        beforeTotal += time_large_alloc();

    std::cout << "Avg time BEFORE fragmentation: "
              << beforeTotal / trials << " µs\n";

    // Fragment the heap
    for (int i = 0; i < allocCount; ++i)
        blocks[i] = malloc(blockSize + (i % 3) * 32);
    for (int i = 0; i < allocCount; i += 2)
        free(blocks[i]);

    double afterTotal = 0;
    for (int i = 0; i < trials; ++i)
        afterTotal += time_large_alloc();

    std::cout << "Avg time AFTER fragmentation:  "
              << afterTotal / trials << " µs\n";

    for (int i = 1; i < allocCount; i += 2)
        free(blocks[i]);

    return 0;
}
