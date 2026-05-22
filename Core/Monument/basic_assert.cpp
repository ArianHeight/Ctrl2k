#include "Monument.h"

compile_assert(NUM_BYTES_IN_32_BITS * 2 == NUM_BYTES_IN_64_BITS);
compile_assert(NUM_BYTES_IN_32_BITS * NUM_BITS_PER_BYTE == 32);