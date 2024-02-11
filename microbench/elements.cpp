//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <multi/vector.hpp>
#include <multi/details/tuple_like.hpp>

#include <benchmark/benchmark.h>

#include <vector>
#include <tuple>
#include <cmath>
#include <ranges>

template<multi::details::tuple_like_element Container>
static void BM_elements_iterate(benchmark::State& state) {
    Container v;
    v.reserve(state.range(0));
    // TODO should depend on Container's value_type
    auto val = std::make_tuple(1.0, 1.0, 1.0, 1.0);
    for(std::size_t _ = 0; _ < state.range(0); ++_)
        v.push_back(val);
    std::tuple_element_t<0, std::ranges::range_value_t<Container>> accum = {};
    for(auto _ : state) {
        for(auto it : v | std::views::elements<0>)
            accum += it;
    }
    benchmark::DoNotOptimize(accum);
}

BENCHMARK(BM_elements_iterate<std::vector<std::tuple<double, double, double, double>>>)->Range(8, std::pow(8,6));
BENCHMARK(BM_elements_iterate<multi::vector<double, double, double, double>>)->Range(8, std::pow(8,6));
