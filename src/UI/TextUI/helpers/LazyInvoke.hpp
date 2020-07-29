#pragma once
#include <any>
#include <vector>
#include <functional>
#include <LazyInvokeHelper.hpp>
#include <utility>

template<typename Function> /* only member functions now supported */
class LazyInvoke
{
public:
    LazyInvoke(Function fn) : m_Fn(fn){}

    template<typename T>
    void addArgument(T&& arg)
    {
        const auto argNum = m_Arguments.size();
        /* will create any with the exact type of 'argNum' argument of m_Fn */
        auto anyMaker = LazyInvokeHelper::correctTypeAnyMaker(argNum, std::forward<T>(arg), m_Fn);
        m_Arguments.push_back(anyMaker());
    }

    void clear()
    {
        m_Arguments.clear();
    }

    template<typename T>
    void invoke(T& calable)
    {
        callImpl(calable, m_Fn);
    }

private:
    template<typename T, typename... Args, typename ArgNumberSeq = std::make_index_sequence<sizeof...(Args)>>
    void callImpl(T& calable, void (T::*)(Args...))
    {
        callImplProxy<Args...>(calable, ArgNumberSeq{});
    }

    template<typename... Args, typename T, std::size_t... ints>
    void callImplProxy(T& calable, std::index_sequence<ints...>)
    {
        std::invoke(m_Fn, calable, getArg<Args>(ints)...);
    }

    template<typename T>
    T getArg(std::size_t argPos)
    {   
        auto it = m_Arguments.begin();
        T arg = std::any_cast<T>(*(it + argPos));
        return arg;
    }

    Function m_Fn;
    std::vector<std::any> m_Arguments{};
};