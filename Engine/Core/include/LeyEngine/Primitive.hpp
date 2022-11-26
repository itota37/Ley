/// @file LeyEngine/Primitive.hpp
/// @copyright (C) 2022 LeyCommunity.
/// @author Taichi Ito.
/// 基本型を提供します。
#ifndef _LEYENGINE_PRIMITIVE_HPP
#define _LEYENGINE_PRIMITIVE_HPP

#include <cstddef>
#include <limits>
#include "LeyEngine/Preprocess.hpp"

/// LeyEngineのすべての機能を含む名前空間です。
namespace LeyEngine 
{
    /// 符号付き8ビット整数型です。
    using I8 = signed char;
    /// 符号無し8ビット整数型です。
    using U8 = unsigned char;
    /// 符号付き16ビット整数型です。
    using I16 = signed short;
    /// 符号無し16ビット整数型です。
    using U16 = unsigned short;
    /// 符号付き32ビット整数型です。
    using I32 = signed int;
    /// 符号無し32ビット整数型です。
    using U32 = unsigned int;
    /// 符号付き64ビット整数型です。
    using I64 = signed long long;
    /// 符号無し64ビット整数型です。
    using U64 = unsigned long long;
    /// 符号付きアドレスサイズの整数型です。
    using ISize = std::ptrdiff_t;
    /// 符号無しアドレスサイズの整数型です。
    using USize = std::size_t;

    /// 32ビット浮動小数点型です。
    using F32 = float;
    /// 64ビット浮動小数点型です。
    using F64 = double;

    /// 論理値型です。
    using Bool = bool;

#if __cplusplus >= 202002L
    /// 文字型です。
    using Char = char8_t
#else
    /// 文字型です。
    using Char = char;
#endif

    /// 型無しを表現する型です。
    using Void = void;

    /// 無を表現する型です。
    using None = std::nullptr_t;

    /// I8の最小値です。
    constexpr I8 I8_MIN = std::numeric_limits<I8>::min();
    /// I8の最大値です。
    constexpr I8 I8_MAX = std::numeric_limits<I8>::max();
    /// U8の最小値です。
    constexpr U8 U8_MIN = std::numeric_limits<U8>::min();
    /// U8の最大値です。
    constexpr U8 U8_MAX = std::numeric_limits<U8>::max();
    /// I16の最小値です。
    constexpr I16 I16_MIN = std::numeric_limits<I16>::min();
    /// I16の最大値です。
    constexpr I16 I16_MAX = std::numeric_limits<I16>::max();
    /// U16の最小値です。
    constexpr U16 U16_MIN = std::numeric_limits<U16>::min();
    /// U16の最大値です。
    constexpr U16 U16_MAX = std::numeric_limits<U16>::max();
    /// I32の最小値です。
    constexpr I32 I32_MIN = std::numeric_limits<I32>::min();
    /// I32の最大値です。
    constexpr I32 I32_MAX = std::numeric_limits<I32>::max();
    /// U32の最小値です。
    constexpr U32 U32_MIN = std::numeric_limits<U32>::min();
    /// U32の最大値です。
    constexpr U32 U32_MAX = std::numeric_limits<U32>::max();
    /// I64の最小値です。
    constexpr I64 I64_MIN = std::numeric_limits<I64>::min();
    /// I64の最大値です。
    constexpr I64 I64_MAX = std::numeric_limits<I64>::max();
    /// U64の最小値です。
    constexpr U64 U64_MIN = std::numeric_limits<U64>::min();
    /// U64の最大値です。
    constexpr U64 U64_MAX = std::numeric_limits<U64>::max();
    /// ISizeの最小値です。
    constexpr ISize ISIZE_MIN = std::numeric_limits<ISize>::min();
    /// ISizeの最大値です。
    constexpr ISize ISIZE_MAX = std::numeric_limits<ISize>::max();
    /// USizeの最小値です。
    constexpr USize USIZE_MIN = std::numeric_limits<USize>::min();
    /// USizeの最大値です。
    constexpr USize USIZE_MAX = std::numeric_limits<USize>::max();

    /// F32の誤差値です。
    constexpr F32 F32_EPSILON = std::numeric_limits<F32>::epsilon();
    /// F32の正の無限大です。
    constexpr F32 F32_INFINITY = std::numeric_limits<F32>::infinity();
    /// F32の負の無限大です。
    constexpr F32 F32_NEGATIVE_INFINITY = -F32_INFINITY;
    /// F64の誤差値です。
    constexpr F64 F64_EPSILON = std::numeric_limits<F64>::epsilon();
    /// F64の正の無限大です。
    constexpr F64 F64_INFINITY = std::numeric_limits<F64>::infinity();
    /// F64の負の無限大です。
    constexpr F64 F64_NEGATIVE_INFINITY = -F64_INFINITY;

    /// 真の値です。
    constexpr Bool YES = true;
    /// 偽の値です。
    constexpr Bool NO = false;

#if __cplusplus >= 202002L
    /// ヌル文字です。
    constexpr Char NULL_CHAR = TXT('\0');
#else
    /// ヌル字型です。
    constexpr Char NULL_CHAR = '\0';
#endif

    /// 無を表現する値です。
    constexpr None NONE = nullptr;

    /// 誤差を考慮して等しいか比較します。
    /// @param l 比較対象です。
    /// @param r 比較対象です。
    /// @retval true 差が誤差の範囲内です。
    /// @retval false 差が誤差の範囲外です。
    inline Bool Equal(F32 l, F32 r)
    {
        return fabsf(l - r) <= F32_EPSILON * fmaxf(1.0f, fmaxf(fabsf(l), fabsf(r)));
    }

    /// 誤差を考慮して等しいか比較します。
    /// @param l 比較対象です。
    /// @param r 比較対象です。
    /// @retval true 差が誤差の範囲内です。
    /// @retval false 差が誤差の範囲外です。
    inline Bool Equal(F64 l, F64 r)
    {
        return fabs(l - r) <= F64_EPSILON * fmax(1.0, fmax(fabs(l), fabs(r)));
    }
}

#endif // !_LEYENGINE_PRIMITIVE_HPP