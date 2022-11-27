// Memory.cpp
// (C) 2022 LeyCommunity.
// author Taichi Ito.

#include <mutex>
#ifdef LEYENGINE_CORE_MODULE
#include <new>
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
class MemoryPool
{
    USize m_elementsCount;     // プールが管理するすべての要素数
    U8 *m_pBuffer;             // バッファ
    USize m_bufferRangeMin;    // バッファの最小アドレス
    USize m_bufferRangeMax;    // バッファの最大アドレス
    USize m_freeElementsCount; // 使用可能な要素数
    U8 **m_ppListTop;

    // コンストラクタ
    // 引数 count 要素数
    // 引数 buffer SIZE * count 分のバッファ
    MemoryPool(USize count, U8* buffer) noexcept
        : m_elementsCount(count)
        , m_pBuffer(buffer)
        , m_freeElementsCount(this->m_elementsCount)
        , m_ppListTop(NONE)
    {
        // バッファの最小、最大アドレスを設定します
        Var top = Cast<USize>(&this->m_buffer[0]);
        Var end = Cast<USize>(&this->m_buffer[(SIZE * this->m_elementsCount)]);
        this->m_bufferRangeMin = top < end ? top : end;
        this->m_bufferRangeMax = top > end ? top : end;

        // 要素のリストを作成します
        //
        // m_pBuffer [elem][elem][elem]...
        //            |  ^  |  ^  |  ^
        //    NONE <--'  '--'  '--'  '-- m_ppListTop
        //
        auto step = SIZE;
        auto length = step * this->m_elementsCount;
        for (USize i = 0; i < length; i += step)
        {
            Var ptr = Cast<U8**>(&this->m_pBuffer[i]);
            *ptr = Cast<U8*>(this->m_ppListTop);
            this->m_freeElementListTop = ptr;
        }
    }

public:

    // 生成します。
    static Result<MemoryPool<SIZE>*, EAllocateError> New(USize count) noexcept
    {
        Var ptr = std::malloc(sizeof(MemoryPool<SIZE>));
        if (ptr == NONE) return EAllocateError::BAD_ALLOCATE;

        Var buffer = Cast<U8*>(std::malloc(SIZE * count));
        if (buffer == NONE) return EAllocateError::BAD_ALLOCATE;

        return new(ptr) MemoryPool<SIZE>(count, buffer);
    }

    // 削除します。
    static Void Delete(MemoryPool<SIZE> *pool) noexcept
    {
        std::free(pool->m_pBuffer);
        std::free(pool);
    }

    // 要素を取得します。
    Void *Allocate() noexcept
    {
        Var ptr = this->m_ppListTop;
        this->m_freeElementsCount -= 1;
        this->m_ppListTop = Cast<U8**>(*ptr);
        return Cast<Void*>(ptr);
    }

    // 要素を戻します。
    Bool Deallocate(Void *pointer) noexcept
    {
        Var ptr = Cast<U8**>(pointer);
        Var adr = Cast<USize>(pointer);
        if (this->m_bufferRangeMin <= adr && adr < this->m_bufferRangeMax)
        {
            this->m_freeElementsCount += 1;
            *ptr = Cast<U8*>(this->m_ppListTop);
            this->m_ppListTop = ptr;
        }
        else
        {
            return NO;
        }
    }

    // 使用可能な要素が無いか判定します。
    Bool IsEmpty() const noexcept
    {
        return this->m_freeElementsCount == 0;
    }

    // すべての要素が使用されていないか判定します。
    Bool IsFull() const noexcept 
    {
        return this->m_freeElementsCount == this->m_elementsCount;
    }
};

// --------------------
//
// 同サイズメモリマネージャ
//
// ====================

template<USize SIZE>
class MemoryPoolManager
{
    USize m_poolCount;                  // プールの
    MemoryPool **m_ppMemoryPools;
    USize m_allocatableMemoryPoolIndex;
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