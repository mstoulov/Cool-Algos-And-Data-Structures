van Emde Boas tree

Implementation with 64-bit unsigned int used as bitsets for lower memory usage

For integers up to 2^k uses a bit more than 2^k/8 bytes

For example for integers < 2^30 (~1e9) uses about 150 Mb
