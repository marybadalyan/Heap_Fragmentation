# Heap_Fragmentation-


Heap fragmentation happens when memory in the heap becomes inefficiently used or broken into small, non-contiguous free blocks over time. This can make it hard for the system to allocate large contiguous blocks of memory, even if there’s technically enough free memory overall.


Types of Heap Fragmentation:
1. External Fragmentation
Occurs when free memory is split into small chunks scattered throughout the heap.
Even if the total free memory is enough, a large allocation can fail because there's no single chunk big enough.


2. Internal Fragmentation
Happens when a block of memory is allocated, but not all of it is used.
The leftover unused space inside the allocated block is wasted.
This can occur due to alignment or block-size rounding.


Why Heap Fragmentation Happens:
Frequent allocation/deallocation of different-sized blocks.

Long-running programs that allocate and free memory in unpredictable patterns.

Lack of memory compaction (common in languages like C/C++ where memory isn't moved automatically).