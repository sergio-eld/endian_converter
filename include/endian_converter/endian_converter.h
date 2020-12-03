#pragma once

#include <type_traits>
#include <cstdint>
#include <utility>
#include <array>
#include <algorithm>

namespace eld
{
    namespace details
    {
        /*!
         * \brief function to get the system's endianness
         * \return true if system uses big endian
         */
        constexpr bool native_is_big_endian()
        {
#ifdef ENDIAN
            return true;
#else
            return false;
#endif
        }

        /*!
 * \brief SFINAE wrapper to get an underlying type from enum.
 * \details If T is not a enum, returns T. Default specialization for T is not enum
 * @tparam T
 */
        template<typename T, bool /*false*/ = std::is_enum<T>::value>
        struct unwrap_enum
        {
            using type = T;
        };

        /*!
 * \brief SFINAE wrapper to get an underlying type from enum.
 * \details Returns underlying type if T is enum.<br>
         * If T is an integral type, returns the same integral type.
 * @tparam T - enum or integral type
 */
        template<typename T>
        struct unwrap_enum<T, true>
        {
            using type = typename std::underlying_type<T>::type;
        };

        /*!
         * \brief Helper type to unwrap enum type.
         * \details Returns underlying type if T is enum.<br>
         * If T is an integral type, returns the same integral type.
         */
        template<typename T>
        using unwrap_enum_t = typename unwrap_enum<T>::type;
    }

    /*!
     * \brief std compliant type for endianness
     * \details
     * If all scalar types are little-endian, std::endian::native equals std::endian::little
     * If all scalar types are big-endian, std::endian::native equals std::endian::big
     */
    enum class endian
    {
        little,
        big,
        native = details::native_is_big_endian() ? big : little
    };

    /*!
     * \brief Basic implementation class for swapping endianness.<br>
     * Used for integral types only. May be used at compile time.
     * @tparam T - Integral type for endian swapping
     */
    template<typename T, bool /*true*/ = std::is_integral<T>::value>
    class swap_endian
    {
        constexpr static size_t sz_minus_one = sizeof(T) - 1;
        template<size_t> struct tag_s
        {
        };

        constexpr static T bitwise_or(tag_s<0>, T original, T res)
        {
            return res | (original >> sz_minus_one * 8);
        }

        template<size_t i>
        constexpr static T bitwise_or(tag_s<i>, T original, T res)
        {
            return bitwise_or(tag_s<i - 1>(), original,
                              res | original << i * 8 >> sz_minus_one * 8 << i * 8);
        }

    public:
        constexpr static T swap(T u)
        {
            return bitwise_or(tag_s<sz_minus_one>(), u, 0);
        }
    };

    /*!
     * \brief Basic implementation class for swapping endianness.<br>
     * Used for all POD and non-integral types. Can't be used at compile time
     * @tparam T - POD or non-integral type for endian swapping
     */
    template<typename T>
    class swap_endian<T, false>
    {
    public:

        static_assert(!std::is_same<T, long double>::value,
                "Long double swapping is not implemented correctly");

        static T swap(const T &u)
        {
            constexpr size_t typeSize = sizeof(T);
            if (typeSize == sizeof(uint8_t))
                return u;

            std::array<uint8_t, typeSize> byteArray{};
            auto begin = reinterpret_cast<const uint8_t *>(&u),
                end = std::next(begin, typeSize);

            // copy to raw byte array
            std::copy(begin, end, byteArray.begin());
            std::reverse(byteArray.begin(), byteArray.end());

            // copy swapped byte array back
            T res{};
            std::copy(byteArray.cbegin(), byteArray.cend(), reinterpret_cast<uint8_t *>(&res));
            return res;
        }
    };

    /*!
     * \brief Convenient function to swap endianness.
     * Can be used at compile time if T is an integral type
     * @tparam T - type for swapping
     * @param u - value for swapping
     * @return Value with swapped endianness
     */
    template<typename T>
    constexpr T swap_endian_v(T u)
    {
        return swap_endian<T>::swap(u);
    }

    /*!
     * \brief Convenient function to change endianness of a value to native.
     * @tparam From - endianness to swap from
     * @tparam T - type for swapping
     * @param u - value for swapping
     * @return Swapped value if native endianness differs from source. Otherwise does nothing.
     */
    template<endian From, typename T>
    constexpr T to_native_endian(T u)
    {
        using U = details::unwrap_enum_t<T>;
        return From == endian::native ? u : static_cast<T>(swap_endian_v(static_cast<U>(u)));
    }

    /*!
     * \brief Convenient function to change endianness of a value from native.
     * @tparam To - target endianness
     * @tparam T - type for swapping
     * @param u - value for swapping
     * @return Swapped value if native endianness differs from requested. Otherwise does nothing.
     */
    template<endian To, typename T>
    constexpr T from_native_endian(T u)
    {
        using U = details::unwrap_enum_t<T>;
        return To == endian::native ? u : static_cast<T>(swap_endian_v(static_cast<U>(u)));
    }
}
