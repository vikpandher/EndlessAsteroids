/* ==========================================================================
   >File: MemoryManager.h
   >Date: 20180713
   >Author: Vik Pandher
   >Details: Here's a memory manager. Allocate huge chunks at a time and 
             divvy them up as needed.
   ========================================================================== */

#pragma once
#include <windows.h>
#include "ErrorCodes.h"



// Memory Block Info Codes For Different Allocations
// -------------------------------------------------------------------------- //
#define INFO_AVAILALBE          0
#define INFO_USED               1

// Maybe in the future I'd like to specify whats using what objects are using
// what memeory.
#define INFO_SCREEN_BUFFER      2
#define INFO_COMMAND_DISPLAY    3
#define INFO_ARRAY              4
#define INFO_STRING             5
#define INFO_G_POINT            6
#define INFO_G_VECTOR           7
#define INFO_G_MATRIX           8
#define INFO_G_LINE             9
#define INFO_G_TRIANGLE         10



// This struct tracks memory blocks. It acts as the header for a block of
// memeory holding info about it.
//
// info = 0, when the block hasn't been allocated
typedef struct block {
    unsigned int size;      // size of memoery block, not including this header
    unsigned char info;     // info indicate if/how this block is being used
    struct block * next;    // pointer to the next block in the memory chain
} Block;



// Simple memory manager. Memory will be stored as block structs. Initially
// a large block will be grabbed, that that block will be divied up by
// being broken up into smaller blocks and allocated to whatever needs some
// memory. When the blocks are returned, I'll try and merge neighboring blocks
// together into bigger blocks.
class MemoryManager
{
    // This tracks the total memory grabbed in bytes
    unsigned int m_totalMemory;

    // Pointer to the first block of memeory
    Block* m_headBlock;

    // Mutex just incase I decide to do something with multiple threads.
    ///HANDLE m_mutex;

public:
    // String
    // ====================================================================== //
    // Constructors and Destructors. Start with 10 mb by default
    MemoryManager(unsigned long size = 10485760);
    ~MemoryManager();

    // allocate
    // ====================================================================== //
    // Marks a block from the totalMemory as unavailable. Large blocks are
    // broken up to the correct size.
    // 
    // @param
    // * unsigned int size, the size of the memory requested in bytes
    // * unsigned char info = 1, used to mark the allocated memory as unavaiable
    // 
    // @return
    // A pointer to the memory block allocated.
    void* allocate(unsigned int size, unsigned char info = INFO_USED);

    // free
    // ====================================================================== //
    // Marks a block from the total Memory as available. Available blocks that
    // are adjacent are merged together.
    // 
    // @param
    // * void* memory, pointer to the memory block being made available
    void free(void* memory);

    // Getter functions used to print memory information
    unsigned int getTotalMemory();
    Block* getHeadBlock();
};

extern MemoryManager g_memoryManager;