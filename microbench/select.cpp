//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <cmath>
#include <tuple>
#include <vector>

#include <benchmark/benchmark.h>

#include <multi/get.hpp>
#include <multi/select.hpp>
#include <multi/vector.hpp>

template <multi::details::tuple_like_element Container>
static void BM_single_select_iterate(benchmark::State& state) {
    Container v;
    v.reserve(state.range(0));
    // TODO should depend on Container's value_type
    auto val = std::make_tuple(1.0, 1.0, 1.0, 1.0);
    for (std::size_t _ = 0; _ < state.range(0); ++_) v.push_back(val);
    std::tuple_element_t<0, typename Container::value_type> accum = {};
    for (auto _ : state) {
        for (auto it : v | multi::select<0>) accum += it;
    }
    benchmark::DoNotOptimize(accum);
}

template <multi::details::tuple_like_element Container>
static void BM_select_iterate(benchmark::State& state) {
    Container v;
    v.reserve(state.range(0));
    // TODO should depend on Container's value_type
    auto val = std::make_tuple(1.0, 1.0, 1.0, 1.0);
    for (std::size_t _ = 0; _ < state.range(0); ++_) v.push_back(val);
    std::tuple<std::tuple_element_t<0, typename Container::value_type>,
               std::tuple_element_t<2, typename Container::value_type>>
        accum = {};
    for (auto _ : state) {
        for (auto it : v | multi::select<0, 2>) {
            multi::get<0>(accum) += multi::get<0>(it);
            multi::get<1>(accum) += multi::get<1>(it);
        }
    }
    benchmark::DoNotOptimize(accum);
}

BENCHMARK(BM_single_select_iterate<
              std::vector<std::tuple<double, double, double, double>>>)
    ->Range(8, std::pow(8, 6));
BENCHMARK(
    BM_single_select_iterate<multi::vector<double, double, double, double>>)
    ->Range(8, std::pow(8, 6));
BENCHMARK(
    BM_select_iterate<std::vector<std::tuple<double, double, double, double>>>)
    ->Range(8, std::pow(8, 6));
BENCHMARK(BM_select_iterate<multi::vector<double, double, double, double>>)
    ->Range(8, std::pow(8, 6));