//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <array>
#include <numeric>
#include <ranges>
#include <tuple>
#include <vector>

#include <benchmark/benchmark.h>

#include <multi/bench/container.hpp>
#include <multi/bench/range.hpp>
#include <multi/bench/views.hpp>
#include <multi/select.hpp>
#include <multi/vector.hpp>

template <multi::details::tuple_like_element Container, auto View>
static void BM_accumulate(benchmark::State& state) {
    auto container =
        multi::bench::container<Container>::allocate(state.range(0));
    auto val = typename Container::value_type(1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
    std::fill(container->begin(), container->begin() + state.range(0), val);
    for (auto _ : state) {
        auto view = *container | View;
        benchmark::DoNotOptimize(
            std::accumulate(view.begin(), view.begin() + state.range(0), 0.0));
    }
}

using multi::bench::elements;
using multi::bench::MAX_RANGE;

BENCHMARK(
    BM_accumulate<
        std::array<std::tuple<double, double, double, double, double, double>,
                   MAX_RANGE>,
        elements<0>>)
    ->Range(8, MAX_RANGE);
BENCHMARK(
    BM_accumulate<
        std::array<std::tuple<double, double, double, double, double, double>,
                   MAX_RANGE>,
        multi::select<0>>)
    ->Range(8, MAX_RANGE);
BENCHMARK(
    BM_accumulate<
        std::vector<std::tuple<double, double, double, double, double, double>>,
        elements<0>>)
    ->Range(8, MAX_RANGE);
BENCHMARK(
    BM_accumulate<
        std::vector<std::tuple<double, double, double, double, double, double>>,
        multi::select<0>>)
    ->Range(8, MAX_RANGE);
BENCHMARK(
    BM_accumulate<multi::vector<double, double, double, double, double, double>,
                  multi::select<0>>)
    ->Range(8, MAX_RANGE);
BENCHMARK(BM_accumulate<multi::array<MAX_RANGE, double, double, double, double,
                                     double, double>,
                        multi::select<0>>)
    ->Range(8, MAX_RANGE);
