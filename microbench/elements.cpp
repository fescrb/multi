//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <array>
#include <cmath>
#include <ranges>
#include <tuple>
#include <vector>

#include <benchmark/benchmark.h>

#include <multi/array.hpp>
#include <multi/details/tuple_like.hpp>
#include <multi/vector.hpp>

template <multi::details::tuple_like_element Container>
static void BM_vector_elements_iterate(benchmark::State& state) {
    Container v;
    v.reserve(state.range(0));
    auto val = typename Container::value_type(1.0, 1.0, 1.0, 1.0);
    for (std::size_t _ = 0; _ < state.range(0); ++_) v.push_back(val);
    std::tuple_element_t<0, std::ranges::range_value_t<Container>> accum = {};
    for (auto _ : state) {
        for (auto it : v | std::views::elements<0>) accum += it;
    }
    benchmark::DoNotOptimize(accum);
}

template <multi::details::tuple_like_element Container>
static void BM_array_elements_iterate(benchmark::State& state) {
    // Larger sizes might not fit in the stack frame
    auto v = new Container;
    auto val = typename Container::value_type(1.0, 1.0, 1.0, 1.0);
    for (std::size_t i = 0; i < v->size(); ++i) (*v)[i] = val;
    std::tuple_element_t<0, std::ranges::range_value_t<Container>> accum = {};
    for (auto _ : state) {
        for (auto it : *v | std::views::elements<0>) accum += it;
    }
    benchmark::DoNotOptimize(accum);
}

BENCHMARK(BM_array_elements_iterate<
          std::array<std::tuple<double, double, double, double>, 8>>);
BENCHMARK(BM_array_elements_iterate<
          std::array<std::tuple<double, double, double, double>, 64>>);
BENCHMARK(BM_array_elements_iterate<
          std::array<std::tuple<double, double, double, double>, 512>>);
BENCHMARK(BM_array_elements_iterate<
          std::array<std::tuple<double, double, double, double>, 4096>>);
BENCHMARK(BM_array_elements_iterate<
          std::array<std::tuple<double, double, double, double>, 32768>>);
BENCHMARK(BM_array_elements_iterate<
          std::array<std::tuple<double, double, double, double>, 262144>>);
/* std::ranges::elements does not work with reference_tuple
BENCHMARK(
    BM_array_elements_iterate<multi::array<8, double, double, double, double>>);
BENCHMARK(BM_array_elements_iterate<
          multi::array<64, double, double, double, double>>);
BENCHMARK(BM_array_elements_iterate<
          multi::array<512, double, double, double, double>>);
BENCHMARK(BM_array_elements_iterate<
          multi::array<4096, double, double, double, double>>);
BENCHMARK(BM_array_elements_iterate<
          multi::array<32768, double, double, double, double>>);
BENCHMARK(BM_array_elements_iterate<
          multi::array<262144, double, double, double, double>>);*/

BENCHMARK(BM_vector_elements_iterate<
              std::vector<std::tuple<double, double, double, double>>>)
    ->Range(8, std::pow(8, 6));
BENCHMARK(
    BM_vector_elements_iterate<multi::vector<double, double, double, double>>)
    ->Range(8, std::pow(8, 6));
