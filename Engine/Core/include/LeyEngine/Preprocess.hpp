/// @file LeyEngine/Preprocess.hpp
/// @copyright (C) 2022 LeyCommunity.
/// @author Taichi Ito.
/// シンボル、マクロを提供します。
#ifndef _LEYENGINE_PREPROCESS_HPP
#define _LEYENGINE_PREPROCESS_HPP

// --------------------
//
// Export
//
// ====================
#if defined(_MSC_VER) && !defined(__gcc__) && !defined(__clang__)
/// extern "C"、__declspec(dllexport) を付加します。
#define EXPORT extern "C" __declspec(dllexport)
#else
/// extern "C" を付加します。
#define EXPORT extern "C"
#endif

// --------------------
//
// Import
//
// ====================
#if defined(_MSC_VER) && !defined(__gcc__) && !defined(__clang__)
/// extern "C" __declspec(dllimport) を付加します。
#define IMPORT extern "C" __declspec(dllimport)
#else
/// extern "C" を付加します。
#define IMPORT extern "C"
#endif

// --------------------
//
// TXT
//
// ====================

/// 標準文字列にエンコードします。
#define TXT(S) u8##S

// --------------------
//
// Var
//
// ====================

/// 推論型です。
#define Var auto

#endif // !_LEYENGINE_PREPROCESS_HPP