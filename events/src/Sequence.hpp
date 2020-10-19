#pragma once

namespace CsEvt
{
//Some boiler plate to get the appropriate placeholders 
// {std::placeholders::_1 ... _N}, when
// working with object bound functions
template <int ... Ns> struct sequence {};

template <int ... Ns> struct seq_gen;

template <int I, int ... Ns>
struct seq_gen<I, Ns...>
{
    using type = typename seq_gen<
        I - 1, I - 1, Ns...>::type;
};

template <int ... Ns>
struct seq_gen<0, Ns...>
{
    using type = sequence<Ns...>;
};

template <int N>
using sequence_t = typename seq_gen<N>::type;
}