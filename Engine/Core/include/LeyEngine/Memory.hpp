/// @file LeyEngine/Memory.hpp
/// @copyright (C) 2022 LeyCommunity.
/// @author Taichi Ito.
/// 基本型を提供します。
#ifndef _LEYENGINE_MEMORY_HPP
#define _LEYENGINE_MEMORY_HPP

#include "LeyEngine/Utility.hpp"

/// LeyEngineのすべての機能を含む名前空間です。
namespace LeyEngine 
{
    /// メモリ確保エラーです。
    enum class EAllocateError
    {
        /// メモリサイズが0でした。
        ZERO_SIZE,
        /// メモリ確保に失敗しました。
        BAD_ALLOCATE,
    };

    /// メモリ解放エラーです。
    enum class EDeallocateError
    {
        /// メモリサイズが0でした。
        ZERO_SIZE,
        /// メモリ確保に失敗しました。
        BAD_DEALLOCATE,
    };

    /// 標準メモリからメモリを確保します。
    /// @param size 確保するバイトサイズです。
    /// @return 確保したメモリのポインタ、または、エラーです。
    Result<Void*, EAllocateError> Allocate(USize size) noexcept;

    /// 標準メモリのメモリを解放します。
    /// @param size 解放するメモリのバイトサイズです。
    /// @param pointer 解放するメモリのポインタです。
    /// @return SUCCESS、または、エラーです。
    Result<Success, EDeallocateError> Deallocate(USize size, Void *pointer) noexcept;

    /// 標準メモリアロケータです。
    /// @tparam T 要素の型です。
    template<typename T>
    class Allocator
    {
    public:

        /// コンストラクタです。
        Allocator()
        {}

        /// コピーコンストラクタです。
        /// @param origin コピー元です。
        Allocator(const Allocator<T> &origin) noexcept
        {}

        /// ムーブコンストラクタです。
        /// @param origin ムーブ元です。
        Allocator(Allocator<T> &&origin) noexcept
        {}

        /// コピー代入します。
        /// @param origin コピー元です。
        Allocator<T> &operator=(const Allocator<T> &origin) noexcept
        {
            return this;
        }

        /// ムーブ代入します。
        /// @param origin ムーブ元です。
        Allocator<T> &operator=(Allocator<T> &&origin) noexcept
        {
            return this;
        }

        /// メモリを確保します。
        /// @param count 要素数です。
        /// @return 確保したポインタ、または、エラーです。
        Result<T*, EAllocateError> Allocate(USize count) noexcept
        {
            Var res = LeyEngine::Allocate(sizeof(T) * count);
            Void *ptr;
            EAllocateError err;
            if (res.IsSuccess(ptr, err))
            {
                return Cast<T*>(ptr);
            }
            else
            {
                return err;
            }
        }

        /// メモリを解放します。
        /// @param count 要素数です。
        /// @param pointer 解放するポインタです。
        /// @return SUCCESS、または、エラーです。
        Result<Success, EDeallocateError> Deallocate(USize count, T *pointer) noexcept
        {
            Var ptr = Cast<Void*>(pointer);
            return LeyEngine::Deallocate(sizeof(T) * count, ptr);
        }
    };
}

#endif // !_LEYENGINE_MEMORY_HPP