#pragma once

#include<functional>
#include<tuple>

#include "Sequence.hpp"

template<int>
struct placeholder_template
{};
namespace std
{
    template<int N>
    struct is_placeholder< placeholder_template<N> >
        : integral_constant<int, N+1> 
    {};
}

namespace CsEvt
{
///<summary>
///Combines a function of any type with an identifier 
///</summary>
template <typename R, typename... ARGS>
class Delegate
{
    friend std::hash<Delegate<R, ARGS...>>;

public:

    //CTors
    Delegate(Delegate const & other) 
        : instanceId(other.instanceId), func(other.func) { }

    Delegate(Delegate&& other)
        :instanceId(other.instanceId), func(std::move(other.func)) { }
    Delegate<R, ARGS...>& operator=(Delegate<R, ARGS...> const & other)
    {
        if(&other == this)
            return *this;

        instanceId = other.instanceId;
        func = std::function<R (ARGS...)>(other.func);

        return *this;
    }

    Delegate<R, ARGS...>& operator=(Delegate<R, ARGS...> && other)
    {
        if(&other == this)
            return *this;

        instanceId = other.instanceId;
        func = std::move(other.func);

        return *this;    
    }


    R operator()(ARGS... args) 
    {
        return func(std::forward<ARGS>(args)...);
    }

    bool operator==(Delegate<R, ARGS...> const & rhs) const noexcept
    {
        return this->instanceId == rhs.instanceId;
    }

    bool operator!=(Delegate<R, ARGS...> const & rhs) const noexcept
    {
        return this->instanceId != rhs.instanceId;
    }


    
    static Delegate MakeDelegate(std::function<R (ARGS...)> func)
    {
        return Delegate(func, instanceCounter++);
    }


    template<class C>
    static Delegate MakeDelegate(C* instance, R (C::*memFunc) (ARGS...))
    {
        return MakeDelegate_(instance, memFunc, CsEvt::sequence_t<sizeof...(ARGS)>{});
    }

private: 
    template<class C, int... indices>
    static Delegate MakeDelegate_(C* instance, R (C::*memFunc) (ARGS...), 
        CsEvt::sequence<indices...>)
    {
        auto fun = std::bind(memFunc, instance, placeholder_template<indices>{}...);
        return Delegate(fun, instanceCounter++);
    }

public:
    int GetId() const { return instanceId; }

private:
    Delegate(std::function<R (ARGS...)> func, int instanceId) 
        : func(func), instanceId(instanceId) {}

    inline static int instanceCounter = 0;

    int instanceId;
    std::function<R (ARGS...)> func;
};
}

namespace std {
    template<typename R, typename... ARGS>
    struct hash<CsEvt::Delegate<R, ARGS...>> {
        size_t operator() (CsEvt::Delegate<R, ARGS...> const &  dlg) const noexcept
        {
            return dlg.instanceId;
        }
    };
};
