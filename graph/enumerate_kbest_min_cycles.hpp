#ifndef ENUMERATE_KBEST_MIN_CYCLES_HPP
#define ENUMERATE_KBEST_MIN_CYCLES_HPP

/*depends on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename S>
vector<edges<S>> enumerate_kbest_min_cycles(graph<S> &, int, int){
    return {};
}

template<typename S>
vector<edges<S>> enumerate_kbest_min_cycles(graph<S> &, int){
    return {};
}
#endif
