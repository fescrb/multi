//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <multi/vector.hpp>

#include <benchmark/benchmark.h>

#include <vector>
#include <tuple>
#include <cmath>

static void BM_std_vector_push_back(benchmark::State& state) {
    for(auto _ : state) {
        std::vector<double> v;
        auto val = 1.0;
        for(std::size_t __ = 0; __ < state.range(0); ++__)
            v.push_back(val);
    }
}

BENCHMARK(BM_std_vector_push_back)->Range(8, std::pow(8,6));

static void BM_multi_vector_push_back(benchmark::State& state) {
    for(auto _ : state) {
        multi::vector<double> v;
        auto val = 1.0;
        for(std::size_t __ = 0; __ < state.range(0); ++__)
            v.push_back(val);
    }
}

BENCHMARK(BM_multi_vector_push_back)->Range(8, std::pow(8,6));

static void BM_std_vector_tuple_push_back(benchmark::State& state) {
    for(auto _ : state) {
        std::vector<std::tuple<double, double, double>> v;
        auto val = std::make_tuple(1.0, 1.0, 1.0);
        for(std::size_t __ = 0; __ < state.range(0); ++__)
            v.push_back(val);
    }
}

BENCHMARK(BM_std_vector_tuple_push_back)->Range(8, std::pow(8,6));

static void BM_multi_vector_tuple_push_back(benchmark::State& state) {
    for(auto _ : state) {
        multi::vector<double, double, double> v;
        auto val = std::make_tuple(1.0, 1.0, 1.0);
        for(std::size_t __ = 0; __ < state.range(0); ++__)
            v.push_back(val);
    }
}

BENCHMARK(BM_multi_vector_tuple_push_back)->Range(8, std::pow(8,6));

static void BM_std_vector_tuple_iterate(benchmark::State& state) {
    std::vector<std::tuple<double, double, double>> v;
    v.reserve(state.range(0));
    auto val = std::make_tuple(1.0, 1.0, 1.0);
    for(std::size_t _ = 0; _ < state.range(0); ++_)
        v.push_back(val);
    auto accum = std::make_tuple(0.0, 0.0, 0.0);
    for(auto _ : state) {
        for(auto it : v)
            std::get<0>(accum) += std::get<0>(it);
    }
}

BENCHMARK(BM_std_vector_tuple_iterate)->Range(8, std::pow(8,6));

static void BM_multi_vector_tuple_iterate(benchmark::State& state) {
    multi::vector<double, double, double> v;
    v.reserve(state.range(0));
    auto val = std::make_tuple(1.0, 1.0, 1.0);
    for(std::size_t _ = 0; _ < state.range(0); ++_)
        v.push_back(val);
    auto accum = std::make_tuple(0.0, 0.0, 0.0);
    for(auto _ : state) {
        for(auto it : v)
            std::get<0>(accum) += std::get<0>(it);
    }
}

BENCHMARK(BM_multi_vector_tuple_iterate)->Range(8, std::pow(8,6));

