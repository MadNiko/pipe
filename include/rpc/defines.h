
#ifndef RPC_DEFINES_H
#define RPC_DEFINES_H


// Определяем платформу сборки
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#   define RPC_OS_WIN
#elif defined(__linux) || defined(__linux__)
#   define RPC_OS_LINUX
#else
#   error("Undefined OS for using RPC module")
#endif


// Определяем компилятор сборки
#if defined(_MSC_VER)
#   define RPC_CC_MSVC _MSC_VER
#elif defined(__MINGW32__)
#   define RPC_CC_MINGW __MINGW32__
#elif defined(__GNUC__)
#   define RPC_CC_GNU
#elif defined(__clang__)
#   define RPC_CC_CLANG __clang__
#else
#   error("Undefined Compiller for using RPC module")
#endif


// Определяем способ сборки/использования библиотеки
#ifndef RPC_EXPORT
#   ifdef RPC_OS_WIN
#       ifndef RPC_LIB_STATIC
#           ifdef RPC_BUILD
#               define RPC_EXPORT __declspec(dllexport)
#           else
#               define RPC_EXPORT __declspec(dllimport)
#           endif
#       else
#           define RPC_EXPORT
#       endif
#   else
#       define RPC_EXPORT
#   endif
#endif


#endif // RPC_DEFINES_H
