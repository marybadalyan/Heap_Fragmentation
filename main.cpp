#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cstring> // for memset
#include <format>   

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr))); // Initial random seed

    const size_t maxBlocks = 10'000'000;
    const size_t minSize = 64 * 1024;    // 64 KB
    const size_t maxSize = 512 * 1024;   // 512 KB
    const size_t iter =  1'000;
    std::vector<void*> heapBlocks;
    heapBlocks.reserve(maxBlocks); // Avoid reallocs

    std::cout << std::format("{:-<87}\n", "-");
    
    std::cout << std::format("| {:<50} | {:<30} |\n", "Description", "Operation Duration (ns)");
    std::cout << std::format("|{:-<52}|{:-<32}|\n", "-", "-");

    // Output for heap operation before fragmentation
    std::cout << std::format("| {:<50} | {:<30} |\n", "Testing heap operations before fragmentation...", "");
    auto start = std::chrono::high_resolution_clock::now();
    void *arr1 = malloc(minSize);
    if (arr1) {
        std::memset(arr1, 0xA5, minSize); // Touch memory to commit
    }
    free(arr1);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long long, std::nano> timeStamp = end - start;
    auto duration = std::chrono::duration<double>(timeStamp).count();
    std::cout << std::format("| {:<50} | {:<30.10f} |\n", "Heap operation before fragmentation", duration);

    // Output for heap pressure test
    std::cout << std::format("| {:<50} | {:<30} |\n", "Starting heap pressure test...", "");

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iter; ++i) {
        int action = rand() % 2; // 0 = allocate, 1 = free

        if (action != 0 || heapBlocks.empty()) {
            size_t size = minSize + (rand() % (maxSize - minSize));
            void* ptr = malloc(size);
            if (ptr) {
                std::memset(ptr, 0xA5, size); // Touch memory to commit
                heapBlocks.push_back(ptr);
            }
        } else {
            // Randomly free one of the blocks
            int index = rand() % heapBlocks.size();
            free(heapBlocks[index]);
            heapBlocks[index] = heapBlocks.back();
            heapBlocks.pop_back();
        }
    }

  
    std::cout << std::format("| {:<50} | {:<30} |\n", "Total allocations (blocks)", heapBlocks.size());

    // Free remaining blocks
    for (void* ptr : heapBlocks) {
        free(ptr);
    }

    end = std::chrono::high_resolution_clock::now();
    timeStamp = end - start;
    duration = std::chrono::duration<double>(timeStamp).count();
    std::cout << std::format("| {:<50} | {:<30.10f} |\n", "Heap pressure loop completed", duration);

  
    std::cout << std::format("| {:<50} | {:<30} |\n", "Testing heap operations after fragmentation...", "");
    start = std::chrono::high_resolution_clock::now();
    void *arr2 = malloc(minSize);
    if (arr2) {
        std::memset(arr2, 0xA5, minSize); 
    }
    free(arr2);
    end = std::chrono::high_resolution_clock::now();
    timeStamp = end - start;
    duration = std::chrono::duration<double>(timeStamp).count();
    std::cout << std::format("| {:<50} | {:<30.10f} |\n", "Heap operation after fragmentation", duration);
    std::cout << std::format("{:-<87}\n", "-");

    return 0;
}
