//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <multi/buffer.hpp>

#include <gtest/gtest.h>

#include <rand.hpp>

TEST(buffer, round_capacity) {
    constexpr std::size_t NUM_TESTS = 100;
    constexpr std::size_t MAX_CAPACITY = 10000;
    using buffer_type = multi::buffer<bool, int, double>;

    for(std::size_t _ = 0; _ < NUM_TESTS; _++) {
        std::size_t capacity = rand() % MAX_CAPACITY; 
        ASSERT_EQ(
            buffer_type::round_capacity(capacity) % buffer_type::alignment, 0
        ) << "Requested capacity: " << capacity;
    }
}

TEST(buffer, empty) {
    multi::buffer<bool, int, double> buff;
    const multi::buffer<bool, int, double>& b_const = buff;

    EXPECT_EQ(buff.data, nullptr);
    EXPECT_EQ(buff.capacity, 0);

    EXPECT_EQ(buff.template column<0>(), nullptr);
    EXPECT_EQ(buff.template column<1>(), nullptr);
    EXPECT_EQ(buff.template column<2>(), nullptr);

    EXPECT_EQ(b_const.template column<0>(), nullptr);
    EXPECT_EQ(b_const.template column<1>(), nullptr);
    EXPECT_EQ(b_const.template column<2>(), nullptr);
}

TEST(buffer, resize) {
    constexpr std::size_t CAPACITY = 100;
    using buffer_type = multi::buffer<bool, int, double>;

    buffer_type buff;
    buff.resize(CAPACITY);
    const buffer_type& b_const = buff;

    EXPECT_NE(buff.data, nullptr);
    EXPECT_GE(buff.capacity, CAPACITY);

    EXPECT_NE(buff.template column<0>(), nullptr);
    EXPECT_NE(buff.template column<1>(), nullptr);
    EXPECT_NE(buff.template column<2>(), nullptr);

    EXPECT_NE(b_const.template column<0>(), nullptr);
    EXPECT_NE(b_const.template column<1>(), nullptr);
    EXPECT_NE(b_const.template column<2>(), nullptr);

    EXPECT_EQ(b_const.template column<0>(), buff.template column<0>());
    EXPECT_EQ(b_const.template column<1>(), buff.template column<1>());
    EXPECT_EQ(b_const.template column<2>(), buff.template column<2>());

    EXPECT_EQ(buff.template column<0>(), reinterpret_cast<bool*>(buff.data));
    EXPECT_EQ(buff.template column<1>(), reinterpret_cast<int*>(buff.data + (CAPACITY * sizeof(bool))));
    EXPECT_EQ(buff.template column<2>(), reinterpret_cast<double*>(buff.data + (CAPACITY * (sizeof(bool)+sizeof(int)))));
}

TEST(buffer, column_access) {
    constexpr std::size_t CAPACITY = 100;
    using buffer_type = multi::buffer<bool, int, double>;

    buffer_type buff;
    buff.resize(CAPACITY);

    const buffer_type& b_const = buff;

    auto bools = rand_buffer<bool>(CAPACITY);
    auto ints = rand_buffer<int>(CAPACITY);
    auto doubles = rand_buffer<double>(CAPACITY);

    for(std::size_t i = 0; i < CAPACITY; i++) {
        buff.template column<0>()[i] = bools[i];
        buff.template column<1>()[i] = ints[i];
        buff.template column<2>()[i] = doubles[i];
    }

    for(std::size_t i = 0; i < CAPACITY; i++) {
        ASSERT_EQ(buff.template column<0>()[i], bools[i]) << "index: " << i;
        ASSERT_EQ(buff.template column<1>()[i], ints[i]) << "index: " << i;
        ASSERT_EQ(buff.template column<2>()[i], doubles[i]) << "index: " << i;
        ASSERT_EQ(b_const.template column<0>()[i], bools[i]) << "index: " << i;
        ASSERT_EQ(b_const.template column<1>()[i], ints[i]) << "index: " << i;
        ASSERT_EQ(b_const.template column<2>()[i], doubles[i]) << "index: " << i;
    }
}

TEST(buffer, full_copy) {
    constexpr std::size_t CAPACITY = 100;
    using buffer_type = multi::buffer<bool, int, double>;

    buffer_type buff;
    buff.resize(CAPACITY);

    auto bools = rand_buffer<bool>(CAPACITY);
    auto ints = rand_buffer<int>(CAPACITY);
    auto doubles = rand_buffer<double>(CAPACITY);

    for(std::size_t i = 0; i < CAPACITY; i++) {
        buff.template column<0>()[i] = bools[i];
        buff.template column<1>()[i] = ints[i];
        buff.template column<2>()[i] = doubles[i];
    }
    
    buffer_type b_copy = buffer_type(buff);

    for(std::size_t i = 0; i < CAPACITY; i++) {
        ASSERT_EQ(buff.template column<0>()[i], bools[i]) << "index: " << i;
        ASSERT_EQ(buff.template column<1>()[i], ints[i]) << "index: " << i;
        ASSERT_EQ(buff.template column<2>()[i], doubles[i]) << "index: " << i;
        ASSERT_EQ(b_copy.template column<0>()[i], bools[i]) << "index: " << i;
        ASSERT_EQ(b_copy.template column<1>()[i], ints[i]) << "index: " << i;
        ASSERT_EQ(b_copy.template column<2>()[i], doubles[i]) << "index: " << i;
    }

    EXPECT_EQ(buff.capacity, b_copy.capacity);

    buffer_type b_copy_by_assign;
    b_copy_by_assign = buff;
    
    for(std::size_t i = 0; i < CAPACITY; i++) {
        ASSERT_EQ(buff.template column<0>()[i], bools[i]) << "index: " << i;
        ASSERT_EQ(buff.template column<1>()[i], ints[i]) << "index: " << i;
        ASSERT_FLOAT_EQ(buff.template column<2>()[i], doubles[i]) << "index: " << i;
        ASSERT_EQ(b_copy_by_assign.template column<0>()[i], bools[i]) << "index: " << i;
        ASSERT_EQ(b_copy_by_assign.template column<1>()[i], ints[i]) << "index: " << i;
        ASSERT_FLOAT_EQ(b_copy_by_assign.template column<2>()[i], doubles[i]) << "index: " << i;
    }

    EXPECT_EQ(buff.capacity, b_copy_by_assign.capacity);
}

TEST(buffer, expand) {
    constexpr std::size_t CAPACITY = 100;
    constexpr std::size_t GROW_FACTOR = 2;
    using buffer_type = multi::buffer<bool, int, double>;

    buffer_type buff;
    buff.resize(CAPACITY);

    auto bools = rand_buffer<bool>(CAPACITY);
    auto ints = rand_buffer<int>(CAPACITY);
    auto doubles = rand_buffer<double>(CAPACITY);

    for(std::size_t i = 0; i < CAPACITY; i++) {
        buff.template column<0>()[i] = bools[i];
        buff.template column<1>()[i] = ints[i];
        buff.template column<2>()[i] = doubles[i];
    }
    
    buff.resize(CAPACITY*GROW_FACTOR);

    ASSERT_EQ(buff.capacity % buffer_type::alignment, 0);
    ASSERT_GE(buff.capacity, CAPACITY*GROW_FACTOR);

    for(std::size_t i = 0; i < CAPACITY; i++) {
        ASSERT_EQ(buff.template column<0>()[i], bools[i]) << "index: " << i;
        ASSERT_EQ(buff.template column<1>()[i], ints[i]) << "index: " << i;
        ASSERT_FLOAT_EQ(buff.template column<2>()[i], doubles[i]) << "index: " << i;
    }
}