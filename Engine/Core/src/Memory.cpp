// Memory.cpp
// (C) 2022 LeyCommunity.
// author Taichi Ito.

#include <mutex>
#ifdef LEYENGINE_CORE_MODULE
#endif
#include "LeyEngine/Memory.hpp"

using namespace LeyEngine;

#ifdef LEYENGINE_CORE_MODULE

// --------------------
// 
// メモリプール
// 
// ====================

template<USize SIZE>
struct MemoryPool
{
    USize maxCount;
    USize freeCount;
    U8 *buffer;
    USize rangeMin;
    USize rangeMax;
    U8 **listTop;

    MemoryPool(USize count)
        : maxCount(count)
        , freeCount(count)
        , buffer(Cast<U8*>(std::malloc(SIZE * count)))
    {
        Var first = Cast<USize>(&this->buffer[0]);
        Var last = Cast<USize>(&this->buffer[(SIZE * count) - 1]);
        this->rangeMin = first < last ? first : last;
        this->rangeMax = first > last ? first : last;

        // 要素のリストを作成します。
    //
    // m_bufferPointer [elem][elem][elem]...
    //                 |  ^  |  ^  |  ^
    //      nullptr <--'  '--'  '--'  '-- m_freeElementListTop
    //
    auto step = SIZE;
    auto length = step * this->m_elementCount;
    for (USize i = 0; i < length; i += step)
    {
        auto ptr = reinterpret_cast<U8**>(&this->m_bufferPointer[i]);
        *ptr = reinterpret_cast<U8*>(this->m_freeElementListTop);
        this->m_freeElementListTop = ptr;
    }
    }
};

// 標準メモリからメモリを確保します。
Result<Void*, EAllocateError> LeyEngine::Allocate(USize size) noexcept
{
}

// 標準メモリのメモリを解放します。
Result<Success, EDeallocateError> LeyEngine::Deallocate(USize size, Void *pointer) noexcept
{
}

#else

Result<Void*, EAllocateError> (*g_allocate)(USize);
Result<Success, EDeallocateError> (*g_deallocate)(USize, Void*);
Result<Void*, EAllocateError> GlobalAllocate(USize size) noexcept
{
    if (size == 0) return EAllocateError::ZERO_SIZE;
    Var ptr = std::malloc(size);
    if (ptr != NONE)
    {
        return ptr;
    }
    else
    {
        return EAllocateError::BAD_ALLOCATE;
    }
}
Result<Success, EDeallocateError> GlobalDeallocate(USize size, Void *pointer) noexcept
{
    if (size == 0) return EDeallocateError::ZERO_SIZE;
    std::free(pointer);
} 
std::once_flag g_initMemorySystemOnceFlag;
Void InitMemorySystem()
{
    g_allocate = &GlobalAllocate;
    g_deallocate = &GlobalDeallocate;
}
EXPORT Void SetMemorySystem(Void *allocator, Void *deallocator)
{
    g_allocate = (Result<Void*, EAllocateError> (*)(USize)) allocator;
    g_deallocate = (Result<Success, EDeallocateError> (*)(USize, Void*))deallocator;
}

// 標準メモリからメモリを確保します。
Result<Void*, EAllocateError> LeyEngine::Allocate(USize size) noexcept
{
    std::call_once(g_initMemorySystemOnceFlag, InitMemorySystem);
    return g_allocate(size);
}

// 標準メモリのメモリを解放します。
Result<Success, EDeallocateError> LeyEngine::Deallocate(USize size, Void *pointer) noexcept
{
    std::call_once(g_initMemorySystemOnceFlag, InitMemorySystem);
    return g_deallocate(size, pointer);
}

#endif