/*
 * C Memory Padding Visualization
 * 
 * This program demonstrates how the C compiler arranges struct members in memory
 * and adds padding bytes to satisfy alignment requirements. It shows both the
 * numerical layout information and a visual representation of memory layout.
 */

#include <stdio.h>   // For printf
#include <stddef.h>  // For offsetof macro
#include <stdalign.h> // For alignof operator (C11)

/* 
 * Simple struct containing two pointers
 * On 64-bit systems: each pointer is 8 bytes, so total size = 16 bytes
 * Alignment requirement = 8 (pointer alignment)
 */
typedef struct Name {
    char* first;  // Pointer to first name string
    char* last;   // Pointer to last name string  
} name_t;

/*
 * Main struct demonstrating memory padding
 * Members are intentionally ordered to show padding effects:
 * - char (1 byte, align 1) followed by
 * - int (4 bytes, align 4) creates 3 bytes of padding
 * - double and name_t both align to 8, so they fit naturally
 */
typedef struct Human {
    char   first_initial;   // 1 byte,  align 1 → placed at offset 0
    int    age;             // 4 bytes, align 4 → placed at offset 4 (3 bytes padding after char)
    double height;          // 8 bytes, align 8 → placed at offset 8 (naturally aligned)
    name_t name;            // 16 bytes, align 8 → placed at offset 16 (naturally aligned)
} human_t;

/*
 * Print size and alignment information for basic types and our struct
 * 
 * sizeof(T)  = number of bytes the type occupies
 * alignof(T) = address alignment requirement (address must be multiple of this)
 * offsetof(struct, field) = byte offset of field within the struct
 */
static void dump_layout(void) {
    // Basic types - show their size and alignment requirements
    printf("sizeof(char)   = %zu, alignof(char)   = %zu\n", sizeof(char),   alignof(char));
    printf("sizeof(int)    = %zu, alignof(int)    = %zu\n", sizeof(int),    alignof(int));
    printf("sizeof(double) = %zu, alignof(double) = %zu\n", sizeof(double), alignof(double));
    printf("sizeof(name_t) = %zu, alignof(name_t) = %zu\n", sizeof(name_t), alignof(name_t));
    printf("\n");
    
    // Struct member offsets - where each field actually ends up in memory
    printf("offsetof(first_initial) = %zu\n", offsetof(human_t, first_initial));
    printf("offsetof(age)           = %zu\n", offsetof(human_t, age));
    printf("offsetof(height)        = %zu\n", offsetof(human_t, height));
    printf("offsetof(name)          = %zu\n", offsetof(human_t, name));
    
    // Overall struct information
    printf("sizeof(human_t)         = %zu\n", sizeof(human_t));
    printf("alignof(human_t)        = %zu\n", alignof(human_t));  // Max alignment of all members
}

/*
 * Create a visual representation of the struct's memory layout
 * 
 * This function builds a character array representing each byte of the struct,
 * marking data bytes with letters and padding bytes with 'P'. It then prints
 * both the byte addresses and the corresponding data/padding layout.
 */
static void visualize(void) {
    size_t sz = sizeof(human_t);        // Total struct size in bytes
    char mem[32] = {0};                 // Array to represent memory layout
    
    // Initialize all bytes as empty (we'll mark them as fields or padding)
    for (size_t i = 0; i < sz; i++) mem[i] = '.';

    // Get the offset of each field within the struct
    size_t oF = offsetof(human_t, first_initial);  // Offset 0
    size_t oA = offsetof(human_t, age);            // Offset 4 (after 3 padding bytes)
    size_t oH = offsetof(human_t, height);         // Offset 8 
    size_t oN = offsetof(human_t, name);           // Offset 16

    // Mark each field's bytes in the memory array
    mem[oF] = 'F';  // first_initial: 1 byte at offset 0
    
    // age: 4 bytes starting at offset 4
    for (size_t i = oA; i < oA + sizeof(int); i++) {
        mem[i] = 'A';
    }
    
    // height: 8 bytes starting at offset 8  
    for (size_t i = oH; i < oH + sizeof(double); i++) {
        mem[i] = 'H';
    }
    
    // name: 16 bytes starting at offset 16
    for (size_t i = oN; i < oN + sizeof(name_t); i++) {
        mem[i] = 'N';
    }

    // Any remaining '.' bytes are padding inserted by the compiler
    for (size_t i = 0; i < sz; i++) {
        if (mem[i] == '.') {
            mem[i] = 'P';  // P = padding byte
        }
    }

    // Print the visualization
    printf("\nByte offsets 0..%zu:\n", sz - 1);
    
    // Print byte numbers (addresses relative to struct start)
    for (size_t i = 0; i < sz; i++) {
        printf("%2zu", i);
    }
    printf("\n");
    
    // Print corresponding field/padding markers
    for (size_t i = 0; i < sz; i++) {
        printf(" %c", mem[i]);
    }
    printf("\n");
    
    printf("Legend: F=first_initial, A=age, H=height, P=padding, N=name\n");
}

/*
 * Main function - demonstrates memory layout analysis
 * 
 * First prints numerical information about sizes, alignments, and offsets,
 * then shows a visual representation of how the struct is laid out in memory.
 */
int main(void) {
    // Print detailed size/alignment/offset information
    dump_layout();
    
    // Show visual memory layout with padding
    visualize();
    
    return 0;
}
