#include "tests.h"

#include <string>
#include <iostream>
#include <vector>

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

void runSearchTests()
{
    std::vector<int> vals;
    for(int i = 0; i < 32; i++)
    {
        vals.push_back(i);
    }

    std::cout << subtestPretext << "Testing linear_search\n";
    for(int i = 0; i < 32; i++)
    {
        assert(linear_search(vals, i) == i);
        assert(linear_search(vals.data(), vals.size(), i) == i);
    }
    assert(linear_search(vals, -1) == INVALID_SIZE_T);
    assert(linear_search(vals, 33) == INVALID_SIZE_T);

    std::cout << subtestPretext << "Testing binary_search\n";
    for(int i = 0; i < 32; i++)
    {
        assert(binary_search(vals, i) == i);
        assert(binary_search(vals.data(), vals.size(), i) == i);
    }
    assert(binary_search(vals, -1) == INVALID_SIZE_T);
    assert(binary_search(vals, 33) == INVALID_SIZE_T);

    std::cout << subtestPretext << "Testing binary_search_nearest\n";
    for(int i = 0; i < 32; i++)
    {
        assert(binary_search_nearest(vals, i) == i);
        assert(binary_search_nearest(vals.data(), vals.size(), i) == i);
    }
    assert(binary_search_nearest(vals, -1) == 0);
    assert(binary_search_nearest(vals, 33) == 32);

    std::vector<int> vals2 = { 1, 3, 3, 3, 6, 6, 7 };
    assert(binary_search_nearest(vals2, 2) == 1);
    assert(binary_search_nearest(vals2, 5) == 4);
    assert(binary_search_nearest(vals2, 3) == 1);
    assert(binary_search_nearest(vals2, 6) == 4);
}

void runDataBucketTests()
{
    std::cout << subtestPretext << "Testing data_bucket\n";
    data_bucket<int> test(5);
    assert(test.capacity() == 5);
    assert(test.size() == 0);
    assert(test.data() != nullptr);
    test[0] = 1;
    test[1] = 2;
    test.size() = 2;
    assert(test.size() == 2);
    data_bucket<int> test2 = std::move(test);
    assert(test2[0] == 1 && test2[1] == 2);
    assert(test.data() == nullptr);
}

void runBasicTests()
{
    std::cout << "\n***********************************\nRunning Tests For Monument...\n";
    runCheckSumTests();
    runSearchTests();
    runDataBucketTests();
}