/// @file LeyEngine/Collections/Array.hpp
/// @copyright (C) 2022 LeyCommunity.
/// @author Taichi Ito.
/// 配列型を提供します。
#ifndef _LEYENGINE_COLLECTIONS_ARRAY_HPP
#define _LEYENGINE_COLLECTIONS_ARRAY_HPP

#include <iterator>
#include "LeyEngine/Memory.hpp"

/// LeyEngineのすべての機能を含む名前空間です。
namespace LeyEngine 
{
    /// ポインタイテレータです。
    template<typename T>
    class PointerIterator
    {
        T *m_element;

    public:

        /// イテレータのカテゴリフラグ型です。
        using iterator_category = std::random_access_iterator_tag;

        /// 要素型の型エイリアスです。
        using value_type = T;

        /// イテレータの移動距離を表現する為の符号付き整数型の型エイリアスです。
        using difference_type = ISize;

        /// 要素型のポインタ型エイリアスです。
        using pointer = T*;
        
        /// 要素型の参照型エイリアスです。
        using reference = T&;

        /// 初期化します。
        /// @param pointer メモリ上の現在地を指すポインタです。
        PointerIterator(T *pointer) noexcept
        : m_element(pointer)
        {}

        /// コピーします。
        /// @param origin コピー元です。
        PointerIterator(const PointerIterator<T> &origin) noexcept
        : m_element(origin.m_element)
        {}

        /// ムーブします。
        /// @param origin ムーブ元です。
        PointerIterator(PointerIterator<T> &&origin) noexcept
        : m_element(origin.m_element)
        {
            origin.m_element = nullptr;
        }

        /// コピー代入します。
        /// @param origin コピー元です。
        PointerIterator<T>& operator=(const PointerIterator<T> &origin) noexcept
        {
            this->m_element = origin.m_element;
            return *this;
        }

        /// ムーブ代入します。
        /// @param origin ムーブ元です。
        PointerIterator<T>& operator=(PointerIterator<T> &&origin) noexcept
        {
            this->m_element = origin.m_element;
            origin.m_element = nullptr;
            return *this;
        }

        /// 指定分進めます。
        /// @param step 進める数です。
        PointerIterator<T>& operator+=(USize step) noexcept
        {
            this->m_element += step;
            return *this;
        }

        /// 指定分戻ります。
        /// @param step 戻る数です。
        PointerIterator<T>& operator-=(USize step) noexcept
        {
            this->m_element -= step;
            return *this;
        }
        
        /// 一つ進めます。
        PointerIterator<T>& operator++() noexcept
        {
            this->m_element += 1;
            return *this;
        }
        
        /// 一つ進めて、進む前のイテレータを返します。
        /// @return 進める前のイテレータです。
        PointerIterator<T> operator++(int) noexcept
        {
            auto iter = *this;
            this->m_element += 1;
            return iter;
        }
        
        /// 一つ戻ります。
        PointerIterator<T>& operator--() noexcept
        {
            this->m_element -= 1;
            return *this;
        }
        
        /// 一つ戻って、戻る前のイテレータを返します。
        /// @return 戻る前のイテレータです。
        PointerIterator<T> operator--(int) noexcept
        {
            auto iter = *this;
            this->m_element -= 1;
            return iter;
        }

        /// 要素にアクセスします。
        /// @exception NullRefarenceException 要素がnullptrの可能性があります。
        T &operator*();

        /// 要素にアクセスします。
        /// @exception NullRefarenceException 要素がnullptrの可能性があります。
        const T &operator*() const;

        /// 要素が同等か比較します。
        /// @param other 比較対象です。
        /// @return 同等の場合、真です。
        Bool operator==(const PointerIterator<T> &other) const noexcept
        {
            return l.m_element == r.m_element;
        }

        /// 要素が不等か比較します。
        /// @param other 比較対象です。
        /// @return 不等の場合、真です。
        Bool operator!=(const PointerIterator<T> &other) const noexcept
        {
            return l.m_element != r.m_element;
        }
    };

    /// 不変ポインタイテレータです。
    template<typename T>
    class ConstPointerIterator
    {
        T *m_element;
    
    public:

        /// イテレータのカテゴリフラグ型です。
        using iterator_category = std::random_access_iterator_tag;

        /// 要素型の型エイリアスです。
        using value_type = const T;

        /// イテレータの移動距離を表現する為の符号付き整数型の型エイリアスです。
        using difference_type = ISize;

        /// 要素型のポインタ型エイリアスです。
        using pointer = const T*;
        
        /// 要素型の参照型エイリアスです。
        using reference = const T&;

        /// 初期化します。
        /// @param pointer メモリ上の現在地を指すポインタです。
        ConstPointerIterator(T *pointer) noexcept
        : m_element(pointer)
        {}

        /// コピーします。
        /// @param origin コピー元です。
        ConstPointerIterator(const ConstPointerIterator<T> &origin) noexcept
        : m_element(origin.m_element)
        {}

        /// ムーブします。
        /// @param origin ムーブ元です。
        ConstPointerIterator(ConstPointerIterator<T> &&origin) noexcept
        : m_element(origin.m_element)
        {
            origin.m_element = nullptr;
        }

        /// コピー代入します。
        /// @param origin コピー元です。
        ConstPointerIterator<T>& operator=(const ConstPointerIterator<T> &origin) noexcept
        {
            this->m_element = origin.m_element;
            return *this;
        }

        /// ムーブ代入します。
        /// @param origin ムーブ元です。
        ConstPointerIterator<T>& operator=(ConstPointerIterator<T> &&origin) noexcept
        {
            this->m_element = origin.m_element;
            origin.m_element = nullptr;
            return *this;
        }

        /// 指定分進めます。
        /// @param step 進める数です。
        ConstPointerIterator<T>& operator+=(USize step) noexcept
        {
            this->m_element += step;
            return *this;
        }

        /// 指定分戻ります。
        /// @param step 戻る数です。
        ConstPointerIterator<T>& operator-=(USize step) noexcept
        {
            this->m_element -= step;
            return *this;
        }
        
        /// 一つ進めます。
        ConstPointerIterator<T>& operator++() noexcept
        {
            this->m_element += 1;
            return *this;
        }
        
        /// 一つ進めて、進む前のイテレータを返します。
        /// @return 進める前のイテレータです。
        ConstPointerIterator<T> operator++(int) noexcept
        {
            auto iter = *this;
            this->m_element += 1;
            return iter;
        }
        
        /// 一つ戻ります。
        ConstPointerIterator<T>& operator--() noexcept
        {
            this->m_element -= 1;
            return *this;
        }
        
        /// 一つ戻って、戻る前のイテレータを返します。
        /// @return 戻る前のイテレータです。
        ConstPointerIterator<T> operator--(int) noexcept
        {
            auto iter = *this;
            this->m_element -= 1;
            return iter;
        }

        /// 要素にアクセスします。
        /// @exception NullRefarenceException 要素がnullptrの可能性があります。
        const T &operator*() const;

        /// 要素が同等か比較します。
        /// @param other 比較対象です。
        /// @return 同等の場合、真です。
        Bool operator==(const ConstPointerIterator<T> &other) const noexcept
        {
            return l.m_element == r.m_element;
        }

        /// 要素が不等か比較します。
        /// @param other 比較対象です。
        /// @return 不等の場合、真です。
        Bool operator!=(const ConstPointerIterator<T> &other) const noexcept
        {
            return l.m_element != r.m_element;
        }
    };

    /// 配列型です。
    /// @tparam T 要素型です。
    /// @tparam A 要素アロケータです。
    template<typename T, typename A = Allocator<T>>
    struct Array
    {
        /// 要素の型です。
        using TElement = T;

        /// アロケータの型です。
        using TAllocator = A;

        /// アロケート時のエラー型です。
        using TAllocateError = typename TAllocator::TAllocateError;

        /// アロケート時のエラー型です。
        using TDeallocateError = typename TAllocator::TDeallocateError;

        /// コピーで起こりうるエラー型です。
        using TCopyError = Variant<typename TAllocateError, typename TDeallocateError>;

    private:
        
        TAllocator m_allocator; // アロケータ
        USize m_elementsLength; // 要素配列長
        USize m_elementsCount;  // 要素数
        TElement *m_pElements;  // 要素配列

        // コンストラクタ
        Array(const TAllocator &allocator, USize elementLength, USize elementCount, TElement *pElements) noexcept
            : m_allocator(allocator)
            , m_elementsLength(elementLength)
            , m_elementsCount(elementCount)
            , m_pElements(pElements)
        {}

    public:
    
        /// 作成します。
        /// @param length 配列長です。
        /// @param allocator アロケータのリザルト値です。
        static Result<Array<TElement, TAllocator>&&, TAllocateError> Create(USize length, const TAllocator &allocator = TAllocator()) noexcept
        {
            TElement* pElements = NONE;
            TAllocateError error;
            Result<TElement*, TAllocateError> buffRes = allocator.Allocate(length);
            if (buffRes.IsSuccess(pElements, error))
            {
                return Move(Array<TElement, TAllocator>(allocator, length, 0, pElements));
            }
            else
            {
                return Move(error);
            }
        }

        /// デストラクタです。
        ~Array() noexcept
        {
            this->m_allocator.Deallocate(this->m_elementsLength, this->m_pElements);
        }

        /// コピー代入します。
        /// @param origin コピー元です。
        /// @return 自身、または、コピーエラーです。
        Result<Array<TElement, TAllocator>&, TCopyError> operator=(const Array<TElement, TAllocator> &origin) noexcept
        {
            
        }
    };
}

#endif // !_LEYENGINE_COLLECTIONS_ARRAY_HPP