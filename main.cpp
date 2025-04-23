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
    const size_t minSize = 1024 * 1000;     
    const size_t maxSize = 1024 * 1100;    
    const size_t iter =  5'000;
    std::vector<void*> heapBlocks;
    heapBlocks.reserve(maxBlocks); // Avoid reallocs

    std::cout << std::format("{:-<87}\n", "-");
    
    std::cout << std::format("| {:<50} | {:<30} |\n", "Description", "Operation Duration (ns)");
    std::cout << std::format("|{:-<52}|{:-<32}|\n", "-", "-");

    // Output for heap operation before fragmentation
    std::cout << std::format("| {:<50} | {:<30} |\n", "Testing heap operations before fragmentation...", "");
    auto start = std::chrono::high_resolution_clock::now();
    void *arr1 = malloc(maxSize);  // Allocation of memory
    if (arr1) {
        std::memset(arr1, 0xA5, maxSize); // Touch memory to commit
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<long long, std::nano> timeStamp = end - start;
    auto duration1 = std::chrono::duration<double>(timeStamp).count();
    std::cout << std::format("| {:<50} | {:<30.10f} |\n", "Heap operation before fragmentation", duration1);

    // Output for heap pressure test
    std::cout << std::format("| {:<50} | {:<30} |\n", "Starting heap pressure test...", "");

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iter; ++i) {
        int action = rand() % 5; // 0-3 = allocate, 4 = free

        if (action != 4 || heapBlocks.empty()) {
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

    end = std::chrono::high_resolution_clock::now();
    timeStamp = end - start;
    auto duration3 = std::chrono::duration<double>(timeStamp).count();
    std::cout << std::format("| {:<50} | {:<30.10f} |\n", "Heap pressure loop completed", duration3);

    std::cout << std::format("| {:<50} | {:<30} |\n", "Testing heap operations after fragmentation...", "");
    start = std::chrono::high_resolution_clock::now();
    void *arr2 = malloc(maxSize);  // Another allocation after fragmentation
    if (arr2) {
        std::memset(arr2, 0xA5, maxSize); 
    }
    
    end = std::chrono::high_resolution_clock::now();
    timeStamp = end - start;
    auto duration2 = std::chrono::duration<double>(timeStamp).count();
    std::cout << std::format("| {:<50} | {:<30.10f} |\n", "Heap operation after fragmentation", duration2);
    std::cout << std::format("| {:<50} | {:<30.2f} |\n", "Heap operation Slowdown", duration2/duration1);

    // Free memory
    free(arr1);
    free(arr2);

    // Free remaining blocks
    for (void* ptr : heapBlocks) {
        free(ptr);
    }
    std::cout << std::format("{:-<87}\n", "-");

    return 0;
}
