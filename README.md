### Heap_Fragmentation

Heap fragmentation happens when memory in the heap becomes inefficiently used or broken into small, non-contiguous free blocks over time. This can make it hard for the system to allocate large contiguous blocks of memory, even if there’s technically enough free memory overall.


Types of Heap Fragmentation:
1. External Fragmentation
Occurs when free memory is split into small chunks scattered throughout the heap.
Even if the total free memory is enough, a large allocation can fail because there's no single chunk big enough.


2. Internal Fragmentation
Happens when a block of memory is allocated, but not all of it is used.
The leftover unused space inside the allocated block is wasted.
This can occur due to alignment or block-size rounding.



## Dependencies

- **C++20 compiler** (for `std::format` and modern features)
- **Standard Template Library (STL)** (for `std::chrono`, etc.)

## Build Instructions

1. Clone the repository:
   ```
   git clone https://github.com/username/Heap_Fragmentation
   ```
2. Navigate to the repository:
   ```
   cd Heap_Fragmentation
   ```
3. Generate build files:
   ```
   cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
   ```
4. Build the project:
   ```
   cmake --build build --config Release
   ```
5. Run the executable from the build directory:
   ```
   ./build/Heap_Fragmentation
   ```


### How does memeory fragmentation result in other future heap operations

1. Reduced Available Memory
Internal Fragmentation: This happens when an allocation is made, but the allocated block of memory is larger than what’s actually needed, leading to wasted space within the block.

As a result, even if the system has "enough" free memory in total, future allocations may fail if the memory is too fragmented to provide a large enough contiguous block.

2. Increased Allocation Time
The allocator has to search through fragmented memory blocks to find a suitable place to allocate new memory. As fragmentation increases, the allocator may have to do more work to find a spot, which could result in longer allocation times.

3. Allocation Failures
If memory becomes too fragmented, even though there seems to be enough free memory in total, future allocation requests may fail because the system can't find a sufficiently large contiguous chunk of free memory.

4. Complicated Deallocation
When freeing memory, the allocator may need to merge adjacent free blocks (if the heap implementation allows coalescing) or deal with fragmented free blocks. This may add overhead to the deallocation process, especially when fragmentation is high.

5. Performance Degradation
Fragmentation can cause the system to allocate more frequently or perform complex memory management operations. This increases CPU usage, leading to overall performance degradation, especially in applications that perform a lot of memory allocation and deallocation.

6. Increased Memory Usage
In some systems, to combat fragmentation, allocators may allocate extra memory to avoid fragmentation. This might increase the overall memory footprint and result in wasted memory that could otherwise be used for other purposes.

7. Garbage Collection  
In managed languages with garbage collection, fragmentation can still play a role in memory management. The garbage collector might need to do additional work to compact memory or to find and reclaim fragmented memory, potentially causing performance hits.

8. Memory Pool Exhaustion
Some systems use memory pools to manage heap memory more efficiently. Fragmentation can lead to pool exhaustion, where the pool can no longer provide the necessary memory for future requests. This can cause failures or require the system to fall back to slower memory allocation strategies.



### Example Output
```

---------------------------------------------------------------------------------------
| Description                                        | Operation Duration (ns)        |
|----------------------------------------------------|--------------------------------|
| Testing heap operations before fragmentation...    |                                |
| Heap operation before fragmentation                | 0.0202407000                   |
| Starting heap pressure test...                     |                                |
| Heap pressure loop completed                       | 11.9830615000                  |
| Testing heap operations after fragmentation...     |                                |
| Heap operation after fragmentation                 | 0.0734613000                   |
| Heap operation Slowdown                            | 3.63                           |
---------------------------------------------------------------------------------------

```
In summary, heap fragmentation makes memory management less efficient, increases the likelihood of allocation failures, and can degrade performance. Effective heap management techniques, such as compaction, defragmentation, or using more sophisticated allocation strategies, are crucial to minimizing these effects.


