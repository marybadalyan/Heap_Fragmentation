#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <cstring> // for memset
#ifndef _WIN32
    #include <unistd.h>
#endif
#include <stdio.h>

constexpr int allocCount = 10000;
constexpr int blockSize = 128 ;  
constexpr int trials = 10;
constexpr size_t largeAllocSize = allocCount * blockSize / 2;

void* touch(void* ptr, size_t size) {
    volatile char* p = static_cast<volatile char*>(ptr);
    for (size_t i = 0; i < size; i += 64)
        if (i < size) p[i] = 0;
    return ptr;
}

double time_large_alloc(bool touch_memory = true) {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    #ifdef _WIN32
        void* ptr = malloc(largeAllocSize);
        if (ptr && touch_memory) touch(ptr, largeAllocSize);

        auto end = high_resolution_clock::now();
        if (ptr) free(ptr);
    #else
        void* original = sbrk(0);
        if (brk(static_cast<char*>(original) + largeAllocSize) == 0) {
            if (touch_memory) touch(original, largeAllocSize);
        } else {
            perror("brk (alloc)");
            return 1;
        }
        auto end = high_resolution_clock::now();
        if (brk(original) != 0) {
            perror("brk (free)");
            return 1;
        }

#endif
    return duration<double, std::micro>(end - start).count();
}

int main() {
    std::vector<void*> blocks(allocCount);

    // Measure allocation before fragmentation
    double beforeTime = 0;
    for (int i = 0; i < trials; ++i)
        beforeTime += time_large_alloc();
    std::cout << "Avg time BEFORE fragmentation: " << beforeTime / trials << " µs\n";

    // Fragmentation: Allocate varying sizes
    for (int i = 0; i < allocCount; ++i)
        blocks[i] = malloc(blockSize + (rand() % 128)); // varying sizes
    for (int i = 0; i < allocCount; i += 2)
        free(blocks[i]); // free half — create fragmentation

    // Measure allocation after fragmentation
    double afterTime = 0;
    for (int i = 0; i < trials; ++i)
        afterTime += time_large_alloc();
    std::cout << "Avg time AFTER fragmentation:  " << afterTime / trials << " µs\n";

    // Clean up remaining
    for (int i = 1; i < allocCount; i += 2)
        free(blocks[i]);

    return 0;
}
