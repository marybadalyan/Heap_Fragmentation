#include <iostream>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <random>

double test_operation(std::size_t size, const std::string& label, std::size_t trials) {
    double total_duration = 0.0;
    for (std::size_t i = 0; i < trials; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        void* ptr = std::malloc(size);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        total_duration += duration;
        std::free(ptr);
    }
    double avg_duration = total_duration / trials;
    std::cout << label << " average allocation time (" << trials << " trials): " 
              << avg_duration << " microseconds" << std::endl;
    return avg_duration;
}

std::vector<void*> simulate_heap_fragmentation(std::size_t num_blocks, std::size_t block_size) {
    std::vector<void*> pointers(num_blocks, nullptr);
    for (std::size_t i = 0; i < num_blocks; ++i) {
        pointers[i] = std::malloc(block_size);
        if (!pointers[i]) {
            std::cerr << "Allocation failed at index " << i << std::endl;
            return pointers;
        }
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    for (std::size_t i = 0; i < num_blocks; ++i) {
        if (dis(gen)) {
            std::free(pointers[i]);
            pointers[i] = nullptr;
        }
    }
    return pointers;
}

void cleanup(std::vector<void*>& pointers) {
    for (std::size_t i = 0; i < pointers.size(); ++i) {
        if (pointers[i]) {
            std::free(pointers[i]);
        }
    }
}

int main() {
    const std::size_t test_block_size = 128;        // Closer to fragment size
    const std::size_t num_blocks = 100000;           // Number of small blocks
    const std::size_t fragment_block_size = 64;     // Size of each small block
    const std::size_t trials = 100000;                // More trials for stability

    test_operation(test_block_size, "Before fragmentation", trials);

    std::cout << "Simulating heap fragmentation..." << std::endl;
    std::vector<void*> pointers = simulate_heap_fragmentation(num_blocks, fragment_block_size);

    test_operation(test_block_size, "After fragmentation", trials);

    cleanup(pointers);

    return 0;
}