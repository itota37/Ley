/// @file LeyEngine/Utility.hpp
/// @copyright (C) 2022 LeyCommunity.
/// @author Taichi Ito.
/// 有用な機能を提供します。
#ifndef _LEYENGINE_UTILITY_HPP
#define _LEYENGINE_UTILITY_HPP

#include <utility>
#include "LeyEngine/Primitive.hpp"

/// LeyEngineのすべての機能を含む名前空間です。
namespace LeyEngine 
{
    /// ムーブします。
    /// @tparam T ムーブする型です。
    /// @param value ムーブする値です。
    /// @return ムーブする値です。
    template<typename T>
    constexpr std::remove_reference<T>::type &&Move(T &&value) noexcept
    {
        return std::move(value);
    }

    /// 左辺値はコピー、右辺値はムーブします。
    /// @tparam T 適用する型です。
    /// @param value 適用する値です。
    /// @return 適用する値です。
    template<typename T>
    constexpr T&& Forward(typename std::remove_reference<T>::type &value) noexcept
    {
        return std::forward(value);
    }
    
    /// 左辺値はコピー、右辺値はムーブします。
    /// @tparam T 適用する型です。
    /// @param value 適用する値です。
    /// @return 適用する値です。
    template<typename T>
    constexpr T&& Forward(typename std::remove_reference<T>::type &&value) noexcept
    {
        return std::forward(value);
    }

    /// @cond LEYDOC_INTERNAL
    /// 外部非公開の機能を含む名前空間です。
    namespace _Internal
    {
        /// キャスト関数オブジェクトの特殊化します。
        /// @tparam T キャストする型です。
        /// @tparam U 元の型です。
        /// @tparam IS_BASE_T TがUのスーパークラスかの判定です。
        /// @tparam IS_BASE_U UがTのスーパークラスかの判定です。
        template<typename T, typename U, Bool IS_BASE_T = std::is_base_of_v<T, U>, Bool IS_BASE_U = std::is_base_of_v<U, T>>
        struct _Cast
        {
            /// キャストします。
            /// @param value キャストする値です。
            T operator()(U value) const noexcept
            {
                return reinterpret_cast<T>(Forward(value))
            }
        };

        /// キャスト関数オブジェクトのstatic_cast特殊化です。
        /// @tparam T キャストする型です。
        /// @tparam U 元の型です。
        /// @tparam IS_BASE_T TがUのスーパークラスかの判定です。
        /// @tparam IS_BASE_U UがTのスーパークラスかの判定です。
        template<typename T, typename U>
        struct _Cast<T, U, YES, YES>
        {
            /// キャストします。
            /// @param value キャストする値です。
            T operator()(U value) const noexcept
            {
                return static_cast<T>(Forward(value))
            }
        };
        
        /// キャスト関数オブジェクトのstatic_cast特殊化です。
        /// @tparam T キャストする型です。
        /// @tparam U 元の型です。
        /// @tparam IS_BASE_T TがUのスーパークラスかの判定です。
        /// @tparam IS_BASE_U UがTのスーパークラスかの判定です。
        template<typename T, typename U>
        struct _Cast<T, U, YES, NO>
        {
            /// キャストします。
            /// @param value キャストする値です。
            T operator()(U value) const noexcept
            {
                return static_cast<T>(Forward(value))
            }
        };

        
        /// キャスト関数オブジェクトのdynamic_cast特殊化です。
        /// @tparam T キャストする型です。
        /// @tparam U 元の型です。
        /// @tparam IS_BASE_T TがUのスーパークラスかの判定です。
        /// @tparam IS_BASE_U UがTのスーパークラスかの判定です。
        template<typename T, typename U>
        struct _Cast<T, U, NO, YES>
        {
            /// キャストします。
            /// @param value キャストする値です。
            T operator()(U value) const noexcept
            {
                return dynamic_cast<T>(Forward(value))
            }
        };
    }
    /// @endcond

    /// キャストします。
    /// @tparam T キャストする型です。
    /// @tparam U 元の型です。
    /// @param value キャストする値です。
    template<typename T, typename U>
    T Cast(U value) noexcept
    {
        return _Internal::_Cast<T, U>{}(Forward(value));
    }

    /// 成功を表現する型です。
    using Success = Bool;
    /// 失敗を表現する型です。
    using Failure = Bool;

    /// 成功を表現する値です。
    constexpr Success SUCCESS = (Success)YES;
    /// 失敗を表現する値です。
    constexpr Failure FAILURE = (Failure)NO;

    /// 関数の戻り値とエラーを同時に返すための型です。
    /// @tparam S 成功時の型です。
    /// @tparam F 失敗時の型です。
    template<typename S, typename F>
    class Result
    {
        union UResult
        {
            U8 ready;   // 初期値
            S success;  // 成功時の値
            F failure;  // 失敗時の値

            UResult()
                : ready(0)
            {}

        }m_value;       // 共用値
        Bool m_isSuccess; // 判定値
        
    public:

        /// コンストラクタです。
        /// @param value 成功時の値です。
        Result(S &&value) noexcept
            : m_value()
            , m_isSuccess(YES)
        {
            this->m_value.success = Move(value);
        }

        /// コンストラクタです。
        /// @param value 失敗時の値です。
        Result(F &&value) noexcept
            : m_value()
            , m_isSuccess(NO)
        {
            this->m_value.failure = Move(value);
        }

        /// ムーブ代入します。
        /// @param origin ムーブ元です。
        /// @return 自身の参照です。
        Result<S, F> &operator=(Result<S, F> &&origin) noexcept
        {
            if (this != origin)
            {
                Var tmpState = Move(this->m_state);
                Var tmpValue = Move(this->m_value);
                this->m_state = Move(origin.m_state);
                this->m_value = Move(origin.m_value);
                origin.m_state = Move(tmpState);
                origin.m_value = Move(tmpValue);
            }
            return *this;
        }

        /// ムーブします。
        /// @param origin ムーブ元です。
        Result(Result<S, F> &&origin) noexcept
        {
            *this = Move(origin);
        }

        /// 成功、失敗を判定して値を参照渡しで返します。
        /// @param success 成功した場合に値が返る参照値です。
        /// @param failure 失敗した場合に値が返る参照値です。
        /// @return 判定値です。
        Bool IsSuccess(S &success, F &failure) noexcept
        {
            if (this->m_isSuccess)
            {
                success = Move(this->m_value.success);
                return YES;
            }
            else
            {
                failure = Move(this->m_value.failure);
                return NO;
            }
        }

        /// 成功、失敗を判定して値を参照渡しで返します。
        /// @param success 成功した場合に値が返る参照値です。
        /// @return 判定値です。
        Bool IsSuccess(S &success) noexcept
        {
            if (this->m_isSuccess)
            {
                success = Move(this->m_value.success);
                return YES;
            }
            else
            {
                return NO;
            }
        }

        /// 成功、失敗を判定して値を参照渡しで返します。
        /// @param failure 失敗した場合に値が返る参照値です。
        /// @return 判定値です。
        Bool IsFailure(F &failure) noexcept
        {
            if (this->m_isSuccess)
            {
                return YES;
            }
            else
            {
                failure = Move(this->m_value.failure);
                return NO;
            }
        }
    };
}

#endif // !_LEYENGINE_UTILITY_HPP