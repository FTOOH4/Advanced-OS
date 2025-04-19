#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 100
#define DEFAULT_MEMORY_SIZE 1048576  // Default to 1MB

// Struct to represent a memory block (either allocated or free)
typedef struct {
    char name[10];      // process name or "Unused"
    int start;          // start address
    int end;            // end address
    int used;           // 1 if allocated, 0 if free
} Block;

Block memory[MAX_PROCESSES]; // memory map (list of blocks)
int mem_size = 0;             // total memory size
int block_count = 0;          // number of current blocks

// Function to initialize memory with a single free block
void initialize_memory(int size) {
    strcpy(memory[0].name, "Unused");
    memory[0].start = 0;
    memory[0].end = size - 1;
    memory[0].used = 0;
    block_count = 1;
    mem_size = size;
}

// Function to print current memory status
void print_status() {
    for (int i = 0; i < block_count; i++) {
        printf("Addresses [%d:%d] %s\n", memory[i].start, memory[i].end, memory[i].name);
    }
}

// Helper to merge adjacent unused blocks
void merge_holes() {
    for (int i = 0; i < block_count - 1; i++) {
        if (memory[i].used == 0 && memory[i + 1].used == 0) {
            memory[i].end = memory[i + 1].end;
            for (int j = i + 1; j < block_count - 1; j++) {
                memory[j] = memory[j + 1];
            }
            block_count--;
            i--;
        }
    }
}

// Allocate memory based on strategy: F (first fit), B (best fit), W (worst fit)
void request_memory(char *name, int size, char strategy) {
    int index = -1;
    int best_size = (strategy == 'W') ? -1 : mem_size + 1;

    for (int i = 0; i < block_count; i++) {
        if (memory[i].used == 0) {
            int hole_size = memory[i].end - memory[i].start + 1;
            if (hole_size >= size) {
                if ((strategy == 'F' && index == -1) ||
                    (strategy == 'B' && hole_size < best_size) ||
                    (strategy == 'W' && hole_size > best_size)) {
                    index = i;
                    best_size = hole_size;
                }
            }
        }
    }

    // If no suitable hole was found, print error and return
    if (index == -1) {
        // This means there's no available memory block large enough for the request
        printf("Error: Not enough memory to allocate for %s\n", name);
        return;
    }

    // Split the block
    Block alloc, hole;
    strcpy(alloc.name, name);
    alloc.start = memory[index].start;
    alloc.end = alloc.start + size - 1;
    alloc.used = 1;

    strcpy(hole.name, "Unused");
    hole.start = alloc.end + 1;
    hole.end = memory[index].end;
    hole.used = 0;

    memory[index] = alloc;
    if (hole.start <= hole.end) {
        for (int j = block_count; j > index + 1; j--) {
            memory[j] = memory[j - 1];
        }
        memory[index + 1] = hole;
        block_count++;
    }
}

// Release memory from a process
void release_memory(char *name) {
    for (int i = 0; i < block_count; i++) {
        if (strcmp(memory[i].name, name) == 0 && memory[i].used == 1) {
            strcpy(memory[i].name, "Unused");
            memory[i].used = 0;
            merge_holes();
            return;
        }
    }
    printf("Error: Process %s not found\n", name);
}

// Compact all memory blocks
void compact_memory() {
    int next_free = 0;
    Block new_memory[MAX_PROCESSES];
    int new_count = 0;

    // Move all allocated blocks to the beginning
    for (int i = 0; i < block_count; i++) {
        if (memory[i].used == 1) {
            int size = memory[i].end - memory[i].start + 1;
            Block b;
            strcpy(b.name, memory[i].name);
            b.start = next_free;
            b.end = next_free + size - 1;
            b.used = 1;
            new_memory[new_count++] = b;
            next_free += size;
        }
    }

    // Add one big unused block at the end
    if (next_free < mem_size) {
        Block b;
        strcpy(b.name, "Unused");
        b.start = next_free;
        b.end = mem_size - 1;
        b.used = 0;
        new_memory[new_count++] = b;
    }

    for (int i = 0; i < new_count; i++) {
        memory[i] = new_memory[i];
    }
    block_count = new_count;
}

int main() {
    int size = DEFAULT_MEMORY_SIZE;
    initialize_memory(size);

    char command[100];
    char cmd[10], pname[10], strat;
    int val;

    //command syntax helper
    printf("\
        'RQ <process> <size> <F/B/W>': request memory with strategy\n\
        'RL <process>': release memory of a process\n\
        'C': compact memory\n\
        'STAT': show memory status\n\
        'X': exit program\n");

    while (1) {
        printf("allocator> ");
        fgets(command, sizeof(command), stdin);

        if (sscanf(command, "%s", cmd) == 1) {
            if (strcmp(cmd, "STAT") == 0) {
                print_status();
            } else if (strcmp(cmd, "C") == 0) {
                compact_memory();
            } else if (strcmp(cmd, "X") == 0) {
                break;
            } else if (strcmp(cmd, "RQ") == 0) {
                sscanf(command, "%*s %s %d %c", pname, &val, &strat);
                request_memory(pname, val, strat);
            } else if (strcmp(cmd, "RL") == 0) {
                sscanf(command, "%*s %s", pname);
                release_memory(pname);
            } else {
                printf("Invalid command\n");
            }
        }
    }

    return 0;
}
