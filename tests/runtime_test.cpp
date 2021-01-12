
#include <gtest/gtest.h>
#include <endian_converter/endian_converter.h>

#include <array>
#include <numeric>
#include <algorithm>

using namespace eld;

template<typename T>
bool test_endian_conversion()
{
    std::array<uint8_t, sizeof(T)> input{},
            expected{},
            output{};

    std::iota(input.begin(), input.end(), 1);
    std::copy(input.rbegin(), input.rend(), expected.begin());

    T from{};
    std::copy(input.cbegin(), input.cend(), reinterpret_cast<uint8_t *>(&from));

    T swapped = swap_endian_v(from);
    uint8_t *begin = reinterpret_cast<uint8_t *>(&swapped),
            *end = std::next(begin, sizeof(T));
    std::copy(begin, end, output.begin());

    return std::equal(expected.cbegin(), expected.cend(),
                      output.cbegin());
}

TEST(EndianConversion, uint8_t)
{
    ASSERT_TRUE(test_endian_conversion<uint8_t>());
}

TEST(EndianConversion, uint16_t)
{
    ASSERT_TRUE(test_endian_conversion<uint16_t>());
}

TEST(EndianConversion, uint32_t)
{
    ASSERT_TRUE(test_endian_conversion<uint32_t>());
}

TEST(EndianConversion, uint64_t)
{
    ASSERT_TRUE(test_endian_conversion<uint64_t>());
}

TEST(EndianConversion, TestFloat)
{
    ASSERT_TRUE(test_endian_conversion<float>());
}

TEST(EndianConversio, TestDouble)
{
    ASSERT_TRUE(test_endian_conversion<double>());
}

struct abc
{
    int a;
    float b;
    int c;
};


TEST(EndianConversio, TestStruct)
{
    ASSERT_TRUE(test_endian_conversion<abc>());
}

//TEST(EndianConversio, TestLongDouble)
//{
//    ASSERT_TRUE(test_endian_conversion<long double>());
//}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}