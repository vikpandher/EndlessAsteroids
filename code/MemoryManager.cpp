/* ==========================================================================
   >File: MemoryManager.cpp
   >Date: 20180713
   >Author: Vik Pandher
   >Details: Here's a memory manager. Allocate huge chunks at a time and
             divvy them up as needed.
   ========================================================================== */

#include "MemoryManager.h"



MemoryManager::MemoryManager(unsigned long size /* = STARTING_MEMEORY_SIZE*/)
{
    // Grab memory
    // Note: HeapAlloc allocated out of the pages for you so, the allocated memory isn't movable.
    // VirtualAlloc must give you back pages, so you get multiples of 4096 bytes.
    m_headBlock = (Block*)VirtualAlloc(
        0,                          // lpAddress, we don't care where the memory is
        size + sizeof(Block),       // dwSize, size of the region in bytes
        MEM_COMMIT,                 // flAllocationType
        PAGE_READWRITE);            // flProtect
    m_headBlock->size = size;
    m_headBlock->info = INFO_AVAILALBE;
    m_headBlock->next = NULL;
    m_totalMemory = size + sizeof(Block);
}


MemoryManager::~MemoryManager()
{
    // Free memory
    VirtualFree(m_headBlock, 0, MEM_RELEASE);
}


void* MemoryManager::allocate(unsigned int size, unsigned char info /* = 1*/)
{
    Block* block = m_headBlock;

    // there is a block
    while (block != NULL)
    {
        // the block is abailable and big enough
        if (block->info == 0 && block->size >= size)
        {
            // the block is big enough to split
            if (block->size > size + sizeof(Block))
            {
                unsigned int blockPointer = (unsigned int)block;
                Block* leftoverBlock = (Block*)(blockPointer + sizeof(Block) + size);
                leftoverBlock->size = block->size - sizeof(Block) - size;
                leftoverBlock->info = INFO_AVAILALBE;
                leftoverBlock->next = block->next;

                block->size = size;
                block->info = info;
                block->next = leftoverBlock;

                void* returnPointer = (void*)(block + 1);
                return returnPointer;
            }
            // the block isn't big enough to split
            else
            {
                block->info = info;
                void* returnPointer = (void*)(block + 1);
                return returnPointer;
            }
        }
        // the block is taken or was too small
        else
        {
            block = block->next;
        }
    }
    // there is no avaialbe block
    throw ERROR_MEMORY_UNAVAILABLE;
    return 0;
}


void MemoryManager::free(void* memory)
{
    // set this block as available
    Block* block = (Block*)memory - 1;
    block->info = INFO_AVAILALBE;

    // go through the block chain and merge all adjacent available blocks
    Block* currBlock = m_headBlock;
    while (currBlock->next != NULL)
    {
        if (currBlock->info == INFO_AVAILALBE && currBlock->next->info == INFO_AVAILALBE)
        {
            currBlock->size += currBlock->next->size + sizeof(Block);
            currBlock->next = currBlock->next->next;
        }
        else
        {
            currBlock = currBlock->next;
        }

        if (currBlock->info != INFO_AVAILALBE && currBlock->info != INFO_USED)
        {
            unsigned char * isInfoSomehowACharacter = &currBlock->info;
            int x = 0;
            x++;
        }
    }
}


unsigned int MemoryManager::getTotalMemory()
{
    return m_totalMemory;
}


Block* MemoryManager::getHeadBlock()
{
    return m_headBlock;
}