
#include <endian_converter/endian_converter.h>

using namespace eld;

int main()
{
    static_assert(uint8_t(0xFA) == swap_endian_v(uint8_t(0xFA)), "Invalid result for endian swapping");
    static_assert(uint16_t(0x00AA) == swap_endian_v(uint16_t(0xAA00)), "Invalid result for endian swapping");
    static_assert(uint16_t(0xF0AA) == swap_endian_v(uint16_t(0xAAF0)), "Invalid result for endian swapping");
    static_assert(uint32_t(0x0000CC00) == swap_endian_v(uint32_t(0x00CC0000)),
                  "Invalid result for endian swapping");
    static_assert(uint32_t(0x6A256575) == swap_endian_v(uint32_t(0x7565256A)),
                  "Invalid result for endian swapping");

    return 0;
}