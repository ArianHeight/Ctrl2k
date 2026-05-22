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
    uint32_t c32 = checksum32(test_string.data(), test_string.length());
    std::cout << c32 << ' ' << checksum32(test_string2.data(), test_string2.length()) << '\n';
    assert(c32 != checksum32(test_string2.data(), test_string2.length()));
    assert(c32 < checksum32(test_string1.data(), test_string1.length()));

    std::cout << subtestPretext << "Testing 64-bit checksum\n";
    uint64_t c64 = checksum64(test_string.data(), test_string.length());
    assert(c64 != checksum64(test_string2.data(), test_string2.length()));
    assert(c64 < checksum64(test_string1.data(), test_string1.length()));
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

void runTypeTests()
{
    std::cout << subtestPretext << "Testing types\n";
    compile_assert(TYPE_CHAR<char> && TYPE_CHAR<wchar_t> && TYPE_CHAR<char8_t> && TYPE_CHAR<char16_t> && TYPE_CHAR<char32_t> &&
        !TYPE_CHAR<int8_t> && !TYPE_CHAR<uint8_t>);
    compile_assert(TYPE_INTEGER<int8_t> && TYPE_INTEGER<int16_t> && TYPE_INTEGER<int32_t> && TYPE_INTEGER<int64_t> &&
        TYPE_INTEGER<uint8_t> && TYPE_INTEGER<uint16_t> && TYPE_INTEGER<uint32_t> && TYPE_INTEGER<uint64_t> &&
        !TYPE_INTEGER<float> && !TYPE_INTEGER<double> && !TYPE_INTEGER<char>);
    compile_assert(TYPE_UNSIGNED_INTEGER<uint8_t> && TYPE_UNSIGNED_INTEGER<uint16_t> && TYPE_UNSIGNED_INTEGER<uint32_t> &&
        TYPE_UNSIGNED_INTEGER<uint64_t> &&
        !TYPE_UNSIGNED_INTEGER<float> && !TYPE_UNSIGNED_INTEGER<double> && !TYPE_UNSIGNED_INTEGER<char> &&
        !TYPE_UNSIGNED_INTEGER<int>);
    compile_assert(TYPE_FLOAT<float> && TYPE_FLOAT<double> && TYPE_FLOAT<long double> &&
        !TYPE_FLOAT<int> && !TYPE_FLOAT<char>);
    compile_assert(TYPE_NUMBER<int> && TYPE_NUMBER<float> && !TYPE_NUMBER<char>);
}

enum class TestFlag : int
{
    F1 = 1 << 0,
    F2 = 1 << 1,
    F3 = 1 << 2,
    F4 = 1 << 3
};
ENUM_CLASS_FULL_OP_GEN(TestFlag);
void runEnumClassMacroTests()
{
    std::cout << subtestPretext << "Testing enum class macros\n";

#define ENUM_COLOURS(ENUM_ENTRY)\
ENUM_ENTRY(RED)\
ENUM_ENTRY(BLUE)\
ENUM_ENTRY(GREEN)
    ENUM_MAP_GEN(Colours, uint16_t, ENUM_COLOURS);
#undef ENUM_COLOURS

    assert(ENUM_CLASS_UNDERLYING(Colours, SIZE) == 3);
    assert(Colours::RED == ENUM_CLASS_CONVERT(Colours, 0));
    assert(ENUM_CLASS_COMPARE(Colours, BLUE, == , 1));
    assert(ENUM_CLASS_COMPARE(Colours, GREEN, > , 1) && ENUM_CLASS_COMPARE(Colours, GREEN, <= , 2));

    Colours c = Colours::BLUE;
    std::string str = ENUM_MAP_STRING(Colours, BLUE);
    assert(str == "BLUE");

    ENUM_CLASS_SET(c, Colours, 0);
    str = ENUM_MAP_INST_STRING(Colours, c);
    assert(str == "RED");
    assert(c == Colours::RED);

    ENUM_CLASS_SET(c, Colours, 2);
    str = ENUM_MAP_INST_STRING(Colours, c);
    assert(str == "GREEN");
    assert(c == Colours::GREEN);

    TestFlag f = TestFlag::F1;
    assert(f < TestFlag::F2);
    assert(f <= 1);
    assert(f >= 0);
    assert(f >= TestFlag::F1);
    assert(f != TestFlag::F2);
    f <<= 1;
    assert(f == TestFlag::F2);
    assert(f << 1 == TestFlag::F3);
    assert(f >> 1 == TestFlag::F1);
    f = TestFlag::F4;
    f >>= 2;
    assert(f == TestFlag::F2);
    f |= TestFlag::F4;
    assert(flag_any_set(f, 3));
    assert(flag_all_set(f, TestFlag::F4 | 2));
    assert(flag_all_set(~f, TestFlag::F1 | TestFlag::F3));
    f ^= TestFlag::F4;
    assert(f == TestFlag::F2);
    f &= TestFlag::F1 | TestFlag::F2;
    assert(f == TestFlag::F2);
    assert(((TestFlag::F1 | TestFlag::F3) & ((TestFlag::F1 | TestFlag::F4) ^ TestFlag::F4)) == 1);
}

void runBasicTests()
{
    std::cout << "\n***********************************\nRunning Tests For Monument...\n";
    runCheckSumTests();
    runSearchTests();
    runDataBucketTests();
    runTypeTests();
    runEnumClassMacroTests();
}