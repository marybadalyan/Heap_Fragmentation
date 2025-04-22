

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr))); //seed for the random number generator

    const size_t maxBlocks = 1000; 
    std::vector<void*> heapBlocks;

    for (int i = 0; i < 1000000000; ++i) {
        int action = rand() % 2; // 0 = allocate, 1 = free

        if (action == 0 || heapBlocks.empty()) {
            size_t size =  64 + (rand() % (64 * 1024));
            void* ptr = malloc(size);
            if (ptr) {
                heapBlocks.push_back(ptr);
                std::memset(ptr, 0, size);
            }
        } else {
            // Randomly free one of the existing blocks
            int index = rand() % heapBlocks.size();
            free(heapBlocks[index]);
            heapBlocks[index] = heapBlocks.back();  //  O(1) removal trick
            heapBlocks.pop_back();
        }

        // Occasionally print stats
        if (i % 10000 == 0) {
            std::cout << "Iteration " << i << ", active blocks: " << heapBlocks.size() << "\n";
        }
    }

    // Free remaining blocks
    for (void* ptr : heapBlocks) {
        free(ptr);
    }

    return 0;
}
