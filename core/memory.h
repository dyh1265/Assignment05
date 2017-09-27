//
//  MemoryArena.h
//  Assignment03
//
//  Created by Андрей Сиразитдинов on 12/08/2017.
//  Copyright © 2017 Андрей Сиразитдинов. All rights reserved.
//

#ifndef MemoryArena_h
#define MemoryArena_h
//Pharr 1015
#include <vector>
#include <stdlib.h>

namespace rt{
    // Memory Declarations
    void *AllocAligned(size_t size);
    template <typename T>
    T *AllocAligned(size_t count) {
        return (T *)AllocAligned(count * sizeof(T));
    }

class MemoryArena {
    public:
        // MemoryArena Public Methods
    MemoryArena(int bs = 32768) {
        blockSize = bs;
        curBlockPos = 0;
        currentBlock = AllocAligned<char>(blockSize);
    }
    void *Alloc(int sz) {
       // Round up sz to minimum machine alignment
        sz = ((sz + 15) & (~15));
        if (curBlockPos + sz > blockSize) {
            usedBlocks.push_back(currentBlock);
            if (availableBlocks.size() && sz <= blockSize) {
                currentBlock = availableBlocks.back();
                availableBlocks.pop_back();
            }
            else
                currentBlock = AllocAligned<char>(std::max(sz, blockSize));
                curBlockPos = 0;
        }
        void *ret = currentBlock + curBlockPos;
        curBlockPos += sz;
        return ret;
    }
    template<typename T> T *Alloc(int count = 1) {
        T *ret = (T *)Alloc(count * sizeof(T));
        for (int i = 0; i < count; ++i)
            new (&ret[i]) T();
        return ret;
    }
    void FreeAll() {
        curBlockPos = 0;
        while (usedBlocks.size()) {
            availableBlocks.push_back(usedBlocks.back());
            usedBlocks.pop_back();
        }
    }
    private:
    int curBlockPos, blockSize;
    char *currentBlock;
    std::vector<char *> usedBlocks, availableBlocks;
    };
}
#endif /* MemoryArena_h */
