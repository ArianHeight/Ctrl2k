#pragma once

constexpr int NUM_BOOLEAN_STATES = 2;
constexpr int NUM_3D_AXIS = 3;
constexpr int NUM_2D_AXIS = 2;

constexpr int NUM_BITS_PER_BYTE = 8;

constexpr int NUM_BITS_FOR_4 = 2;
constexpr int NUM_BYTES_IN_32_BITS = 1 << NUM_BITS_FOR_4;
constexpr int NUM_BITS_FOR_8 = 3;
constexpr int NUM_BYTES_IN_64_BITS = 1 << NUM_BITS_FOR_8;

constexpr size_t MASK_1_BIT = (1 << 1) - 1;
constexpr size_t MASK_2_BIT = (1 << 2) - 1;
constexpr size_t MASK_3_BIT = (1 << 3) - 1;
constexpr size_t MASK_4_BIT = (1 << 4) - 1;
constexpr size_t MASK_5_BIT = (1 << 5) - 1;

constexpr size_t INVALID_SIZE_T = SIZE_MAX;