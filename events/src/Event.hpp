#pragma once

#include<vector>
#include<functional>
#include<tuple>

#include "Delegate.hpp"

template <typename... ARGS>
class Event
{
    using EventFunc = Delegate<void, ARGS...>;

public: 
    Event() {}

    int Register(EventFunc e)
    {
        listeners.push_back(e); 
        return e.GetId();       
    }

    template<class C>
    int Register(C* instance, void (C::*memFunc) (ARGS...))
    {
        auto delegate = Delegate<void, ARGS...>::MakeDelegate<C>(instance, memFunc);
        listeners.push_back(delegate);

        return delegate.GetId();
    }
    
    void Unregister(EventFunc e) 
    {
        auto newVectorEnd = std::remove(listeners.begin(), listeners.end(), e);
        listeners.erase(newVectorEnd, listeners.end());
    }

    void Unregister(int handle)
    {
        auto newVectorEnd = std::remove_if(listeners.begin(), listeners.end(), 
            [handle](EventFunc const& evtFunc) -> bool
            {
                return evtFunc.GetId() == handle;
            });
        listeners.erase(newVectorEnd, listeners.end());      
    }

    void Invoke(ARGS... args)
    {
        for(auto lst : listeners)
            lst(std::forward<ARGS>(args)...);
    }

    size_t NumListeners()
    {
        return listeners.size();
    }

    int operator +=(std::function<void (ARGS...)> func)
    {
        auto delegate = Delegate<void, ARGS...>::MakeDelegate(func);
        Register(delegate);

        return delegate.GetId();
    }

    template<typename C>
    int operator +=(std::tuple<C*, void (C::*) (ARGS...)> func)
    {
        return Register(std::get<0>(func), std::get<1>(func));
    }

    void operator-=(int handle)
    {
        Unregister(handle);
    }


private:
    std::vector<EventFunc> listeners;
};