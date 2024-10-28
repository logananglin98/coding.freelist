#include "freeList.h"

freeList::freeList(long int* inRAM, int size) {
    head = inRAM;               // Set head to start of RAM
    head[0] = size - 2;         // First block’s size, excluding header
    head[1] = 0;                // No next free block (NULL)
}

long int* freeList::reserve_space(int size) {
    long int* loc = nullptr;    // To hold the reserved location
    long int* prev = nullptr;
    long int* curr = head;

    // Find the first block with sufficient space
    while (curr != nullptr && curr[0] < size) {
        prev = curr;
        curr = (long int*)curr[1];
    }

    // If a suitable block was found
    if (curr != nullptr) {
        loc = curr + 2;           // Return the start of the usable space
        int remaining = curr[0] - size - 2; // Remaining space after allocation

        if (remaining > 0) {
            // Update the current block to point to the remaining space
            long int* newBlock = curr + size + 2;
            newBlock[0] = remaining;
            newBlock[1] = curr[1];

            // Link the previous block to newBlock if there is one
            if (prev) {
                prev[1] = (long int)newBlock;
            } else {
                head = newBlock;
            }
        } else {
            // No leftover space, so unlink the current block entirely
            if (prev) {
                prev[1] = curr[1];
            } else {
                head = (long int*)curr[1];
            }
        }

        curr[0] = size;           // Set the size of the allocated block
        curr[1] = 0;              // Clear the next pointer (safety)
    }

    return loc;
}

void freeList::free_space(long int* location) {
    long int* header = location - 2;  // Get the header of this block

    // Insert this block at the front of the free list
    header[1] = (long int)head;
    head = header;
}

void freeList::coalesce_forward() {
    long int* curr = head;

    while (curr != nullptr && curr[1] != 0) {
        long int* next = (long int*)curr[1];

        // If current and next blocks are adjacent in memory, coalesce them
        if (curr + curr[0] + 2 == next) {
            curr[0] += next[0] + 2;   // Combine sizes (+2 for the header)
            curr[1] = next[1];        // Link to the next free block
        } else {
            curr = next;              // Move to the next block
        }
    }
}

// print() is given to you. It traverses the free list and prints out what is there.
void freeList::print() {
  cout << "Printing:\n";
  long int * printHead = head;
  
  while( printHead != NULL ) {
    cout << "at " << printHead << "("<< dec <<printHead[0] << " : 0x" << hex << printHead[1] <<  ")\n";
    printHead = (long int *)printHead[1];
  }
}
