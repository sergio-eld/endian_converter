# C++11 Endianness conversion library

This is a header-only template library for conversion of endianness of C++ types.

The library does not utilize common practices of type punning via `reinterpret_cast`
or `union`, which results in undefined behavior.
`swap_endian_v`, `from_native_endian` and `to_native_endian` can be evaluated at 
compile time if converting an integral type or an enum.
Conversion for floating point types and user-defined POD types is possible only
at compile time (prior to C++20 at least).

# TODO: 
- fill this readme
- implement long double conversion?