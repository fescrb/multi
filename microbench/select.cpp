//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <multi/vector.hpp>
#include <multi/select.hpp>

#include <benchmark/benchmark.h>

#include <vector>
#include <tuple>
#include <cmath>

static void BM_multi_vector_tuple_select_iterate(benchmark::State& state) {
    multi::vector<double, double, double> v;
    v.reserve(state.range(0));
    auto val = std::make_tuple(1.0, 1.0, 1.0);
    for(std::size_t _ = 0; _ < state.range(0); ++_)
        v.push_back(val);
    auto accum = std::make_tuple(0.0, 0.0, 0.0);
    for(auto _ : state) {
        for(auto it : v | multi::select<0>)
            std::get<0>(accum) += std::get<0>(it);
    }
    benchmark::DoNotOptimize(accum);
}

BENCHMARK(BM_multi_vector_tuple_select_iterate)->Range(8, std::pow(8,6));
