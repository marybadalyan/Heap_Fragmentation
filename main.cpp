#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <cstring> // for memset
#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
#endif
#include <stdio.h>

constexpr int allocCount = 10000;
constexpr int blockSize = 128;  
constexpr int trials = 100;
constexpr size_t largeAllocSize = allocCount * blockSize * 10;

#ifdef _WIN32
    HANDLE heap = GetProcessHeap();  // Get the default process heap
#endif

void* touch(void* ptr, size_t size) {
    volatile char* p = static_cast<volatile char*>(ptr);
    for (size_t i = 0; i < size; i += 64)
        if (i < size) p[i] = 0;
    return ptr;
}

double time_large_alloc(size_t alloc_size, bool touch_memory = true) { // if true memeory will be commited
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    #ifdef _WIN32
        void* ptr = HeapAlloc(heap, 0, alloc_size);
        if (ptr && touch_memory) touch(ptr, alloc_size);
        if (ptr) HeapFree(heap, 0, ptr);  // Freeing with HeapFree for Windows
    #else
        void* original = sbrk(0);
        if (brk(static_cast<char*>(original) + alloc_size) == 0) {
            if (touch_memory) touch(original, alloc_size);
        } else {
            perror("brk (alloc)");
            return 1;
        }
        // Note: For Linux brk, we don't need to explicitly "free" after the allocation.
    #endif
    auto end = high_resolution_clock::now();
    return duration<double, std::micro>(end - start).count();
}

int main() {
    std::vector<void*> blocks(allocCount);
    std::vector<void*> orignials(allocCount);

    // Measure allocation before fragmentation (with memory touch)
    double beforeTime = 0;
    for (int i = 0; i < trials; ++i)
        beforeTime += time_large_alloc(largeAllocSize, true);  // Touch memory before fragmentation
    std::cout << "Avg time BEFORE fragmentation (touch): " << beforeTime / trials << " µs\n";

    // Fragmentation: Allocate varying sizes
    #ifdef _WIN32
        for (int i = 0; i < allocCount; ++i)
            blocks[i] = HeapAlloc(heap, 0, blockSize + (rand() % 128)); // varying sizes
        for (int i = 0; i < allocCount; i += 2)
            HeapFree(heap, 0, blocks[i]); // free half — create fragmentation
    #else
        for (int i = 0; i < allocCount; ++i) {
            int random_num = blockSize + (rand() % 128);
            void* original = sbrk(0);
            orignials.push_back(original);
            if (brk(static_cast<char*>(original) + random_num) == 0) {
                touch(original, random_num);
            } else {
                perror("brk (alloc)");
                return 1;
            }
        }
        for (int i = 0; i < allocCount; i += 2) {
            if (brk(orignials[i]) != 0) {
                perror("brk (free)");
                return 1;
            }
        }
    #endif

    // Measure allocation after fragmentation (with memory touch)
    double afterTime = 0;
    for (int i = 0; i < trials; ++i)
        afterTime += time_large_alloc(largeAllocSize, true);  // Touch memory after fragmentation
    std::cout << "Avg time AFTER fragmentation (touch): " << afterTime / trials << " µs\n";

    // Clean up remaining memory
    #ifdef _WIN32
        for (int i = 1; i < allocCount; i += 2)
            HeapFree(heap, 0, blocks[i]);
    #else
        for (int i = 1; i < allocCount; i += 2)
            if (brk(orignials[i]) != 0) {
                perror("brk (free)");
                return 1;
            }
    #endif

    return 0;
}
