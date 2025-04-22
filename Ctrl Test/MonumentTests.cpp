#include "tests.h"

#include <string>
#include <iostream>

#include "Core/Monument/Monument.h"

void runCheckSumTests()
{
    const std::string test_string = "Hello, checking, one, two, three, four";
    const std::string test_string1 = "Hello, checking, one, two, three, fout";
    const std::string test_string2 = "This is not the previous string";

    std::cout << subtestPretext << "Testing 32-bit checksum\n";
    hash32_t hash32 = checksum32(test_string.data(), test_string.length());
    assert(hash32 == checksum32(test_string.data(), test_string.length(), 0));
    assert(hash32 != checksum32(test_string2.data(), test_string2.length()));
    assert(hash32 < checksum32(test_string1.data(), test_string1.length()));
    assert(hash32 != checksum32(test_string.data(), test_string.length(), checksum32(test_string2.data(), test_string2.length())));

    std::cout << subtestPretext << "Testing 64-bit checksum\n";
    hash64_t hash64 = checksum64(test_string.data(), test_string.length());
    assert(hash64 == checksum64(test_string.data(), test_string.length(), 0));
    assert(hash64 != checksum64(test_string2.data(), test_string2.length()));
    assert(hash64 < checksum64(test_string1.data(), test_string1.length()));
    assert(hash64 != checksum64(test_string.data(), test_string.length(), checksum64(test_string2.data(), test_string2.length())));

    std::cout << subtestPretext << "Testing 128-bit checksum\n";
    hash128_t hash128 = checksum128(test_string.data(), test_string.length());
    assert(hash128 == checksum128(test_string.data(), test_string.length(), { 0, 0 }));
    assert(hash128 != checksum128(test_string2.data(), test_string2.length()));
    assert(hash128 < checksum128(test_string1.data(), test_string1.length()));
    assert(hash128 != checksum128(test_string.data(), test_string.length(), checksum128(test_string2.data(), test_string2.length())));

    std::cout << subtestPretext << "Testing 256-bit checksum\n";
    hash256_t hash256 = checksum256(test_string.data(), test_string.length());
    assert(hash256 == checksum256(test_string.data(), test_string.length(), { 0, 0, 0, 0 }));
    assert(hash256 != checksum256(test_string2.data(), test_string2.length()));
    assert(hash256 < checksum256(test_string1.data(), test_string1.length()));
    assert(hash256 != checksum256(test_string.data(), test_string.length(), checksum256(test_string2.data(), test_string2.length())));
}

void runBasicTests()
{
    std::cout << "\n***********************************\nRunning Tests For Monument...\n";
    runCheckSumTests();
}