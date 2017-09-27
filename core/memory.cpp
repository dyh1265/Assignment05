//
//  memory.cpp
//  Assignment03
//
//  Created by Андрей Сиразитдинов on 13/08/2017.
//  Copyright © 2017 Андрей Сиразитдинов. All rights reserved.
//

#include "memory.h"

namespace rt {
    
    // Memory Allocation Functions
    void *AllocAligned(size_t size) {
        void *ptr;
        if (posix_memalign(&ptr, 64, size) != 0) ptr = nullptr;
        return ptr;
    }
    
    void FreeAligned(void *ptr) {
        if (!ptr) return;
        free(ptr);
    }
}
