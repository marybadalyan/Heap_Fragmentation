#include <cstdlib>
#include <vector>
#include <iostream>
#include <chrono>

int main() {
    const int allocCount = 1000;
    const int blockSize = 1024; // 1 KB blocks
    const size_t largeSize = blockSize * allocCount / 2;

    std::vector<void*> blocks(allocCount);

    // Step A: Time large alloc BEFORE fragmentation
    auto t1 = std::chrono::high_resolution_clock::now();
    void* largeBefore = malloc(largeSize);
    auto t2 = std::chrono::high_resolution_clock::now();

    if (largeBefore)
        std::cout << "Large alloc BEFORE fragmentation succeeded.\n";
    else
        std::cout << "Large alloc BEFORE fragmentation failed.\n";

    std::chrono::duration<double, std::micro> elapsed1 = t2 - t1;
    std::cout << "Time taken: " << elapsed1.count() << " ms\n";

    free(largeBefore); // Free the first large alloc

    // Step B: Fragment the heap
    for (int i = 0; i < allocCount; ++i)
        blocks[i] = malloc(blockSize + (i % 3) * 32);  // Vary sizes

    for (int i = 0; i < allocCount; i += 2)
        free(blocks[i]);

    // Step C: Time large alloc AFTER fragmentation
    auto t3 = std::chrono::high_resolution_clock::now();
    void* largeAfter = malloc(largeSize);
    auto t4 = std::chrono::high_resolution_clock::now();

    if (largeAfter)
        std::cout << "Large alloc AFTER fragmentation succeeded.\n";
    else
        std::cout << "Large alloc AFTER fragmentation failed.\n";

    std::chrono::duration<double, std::micro> elapsed2 = t4 - t3;
    std::cout << "Time taken: " << elapsed2.count() << " ms\n";

    free(largeAfter); // Cleanup

    // Final cleanup of remaining allocations
    for (int i = 1; i < allocCount; i += 2)
        free(blocks[i]);

    return 0;
}
