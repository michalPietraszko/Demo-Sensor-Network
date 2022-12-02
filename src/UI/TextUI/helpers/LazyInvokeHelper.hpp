#pragma once
#include <any>
#include <cassert>
#include <functional>
#include <type_traits>
#include <utility>

class LazyInvokeHelper {
    template <typename T, typename Arg>
    static std::any returnHelper(Arg arg) {
        if constexpr (std::is_constructible_v<std::any, std::in_place_type_t<T>, Arg>) {
            return std::make_any<T>(arg);
        } else {
            return {};
        }
    }

    template <typename First, typename... Args, typename Arg>
    static std::function<std::any()> maker(int n, Arg arg) {
        if (n == 0) return [arg]() { return returnHelper<First>(arg); };
        n--;
        // clang-format off
        if constexpr (not sizeof...(Args)) 
            return [arg](){ assert(false); return returnHelper<First>(arg); };
        else
            return maker<Args...>(n, arg); // clang-format on
    }

public:
    template <typename Ret, typename Arg, typename Type, typename... Args>
    static auto correctTypeAnyMaker(int n, Arg arg, Ret (Type::*func)(Args...)) {
        return maker<Args...>(n, arg);
    }
};
