#include "tests.h"

#include <iostream>

#include "Core/AncestralHall/ahl.h"

void runStackMaskedArrayTests()
{
    std::cout << subtestPretext << "Testing stack masked array\n";

    ahl::stack_maskedarray<int, 16> arr1;
    assert(arr1.empty());
    assert(arr1.size() == 0);
    assert(arr1.fill(3, 15));
    assert(arr1.fill(15, 2));
    assert(arr1.fill(6, 5));
    assert(!arr1.fill(6, 10));
    assert(arr1.size() == 3);
    assert(arr1[6] == 5);
    assert(arr1.at(15) == 2);
    arr1.fill_or_overwrite(8, 8);
    arr1.fill_or_overwrite(6, 12);
    assert(arr1.size() == 4);
    assert(arr1[6] == 12);
    assert(arr1[8] == 8);
    assert(!arr1.empty());
    arr1.erase(6);
    arr1.erase(7);
    assert(arr1.size() == 3);
    assert(!arr1.is_filled(6));
    assert(arr1.is_filled(8));

    ahl::stack_maskedarray<int, 16> arr2 = arr1;
    assert(arr2 == arr1);
    ahl::stack_maskedarray<int, 16> arr3;
    arr3 = std::move(arr2);
    assert(arr2.empty());
    assert(arr1 == arr3);
    arr3.clear();
    assert(arr3.empty());
    assert(arr1 != arr3);
}

void runFixedVectorTests()
{
    std::cout << subtestPretext << "Testing fixed vector\n";

    ahl::stack_vector<int, 8> v1;
    assert(v1.size() == 0);
    assert(v1.capacity() == 8);
    assert(v1.empty());

    v1.push_back(1);
    assert(v1.size() == 1);
    assert(v1[0] == 1);
    assert(!v1.empty());
    assert(v1.capacity() == 8);

    v1[0]++;
    assert(v1.at(0) == 2);

    v1.front()++;
    assert(v1.back() == 3);

    v1.back()++;
    assert(v1.front() == 4);

    for(int i = 0; i < 7; i++)
    {
        int& val = v1.emplace_back();
        val = i;
    }
    assert(v1.back() == 6);
    assert(v1.front() == 4);
    assert(v1.size() == 8);

    v1.pop_back();
    assert(v1.size() == 7);
    assert(v1.capacity() == 8);

    v1.push_back(20);
    assert(v1[7] == 20);
    assert(v1.size() == 8);
    assert(sizeof(v1) == sizeof(size_t) + sizeof(int) * 8);

    v1.clear();
    assert(v1.empty());
    for(int i = 0; i < 5; ++i)
    {
        v1.push_back(i);
    }
    ahl::stack_vector<int, 6> v2 = v1;

    assert(v1.size() == 5);
    v1.erase(2);
    v1.erase(0);
    v1.erase(2);
    assert(v1.size() == 2);
    assert(v1[0] == 1 && v1[1] == 3);
    v1.insert(0, 0);
    v1.insert(3, 4);
    v1.insert(2, 2);
    assert(v1 == v2);
}

void runStackSetTests()
{
    std::cout << subtestPretext << "Testing stack set\n";

    ahl::stack_set<int, 8> set1;
    assert(set1.empty());
    assert(set1.capacity() == 8);
    set1.insert(5);
    set1.insert(2);
    set1.insert(7);
    set1.insert(7);
    assert(set1.size() == 3);
    assert(!set1.empty());
    assert(set1.contains(7));
    assert(set1.contains(5));
    assert(set1.contains(2));
    assert(set1.at_index(1) == 5);
    assert(set1.at_index(2) == 7);
    assert(!set1.contains(3));
    set1.erase(3);
    assert(set1.size() == 3);
    set1.erase(2);
    assert(set1.size() == 2);
    assert(!set1.contains(2));

    ahl::stack_set<int, 16> set2 = set1;
    assert(!set1.empty());
    assert(set1 == set2);
    ahl::stack_set<int, 12> set3 = std::move(set2);
    assert(set2.empty());
    assert(set3 == set1);

    set3.clear();
    assert(set3.empty());
}

void runStackMapTests()
{
    std::cout << subtestPretext << "Testing stack map\n";

    ahl::stack_map<int, int, 16> map1;
    assert(map1.empty());
    assert(map1.size() == 0);

    assert(map1.insert(26, 12) != INVALID_SIZE_T);
    assert(map1.insert(-1, 200) != INVALID_SIZE_T);
    assert(map1.insert(100, 0) != INVALID_SIZE_T);
    assert(map1.insert(100, 20) == INVALID_SIZE_T);
    assert(map1.find_index(100) == 2);
    assert(map1[100] == 0);
    assert(map1.at(-1) == 200);
    assert(map1.insert_or_assign(100, 30) != INVALID_SIZE_T);
    assert(map1.size() == 3);
    assert(map1[100] == 30);
    assert(map1.insert_or_assign(50, 64) != INVALID_SIZE_T);
    assert(map1.size() == 4);
    
    assert(map1.at_index(0).key == -1 && map1.at_index(0).value == 200);
    assert(map1.at_index(1).key == 26 && map1.at_index(1).value == 12);
    assert(map1.at_index(2).key == 50 && map1.at_index(2).value == 64);
    assert(map1.at_index(3).key == 100 && map1.at_index(3).value == 30);

    ahl::stack_map<int, int, 8> map2;
    map2 = map1;
    assert(map2 == map1);
    ahl::stack_map<int, int, 8> map3 = std::move(map2);
    assert(map2.empty());
    assert(map1 == map3);

    map3.clear();
    assert(map3.empty());
}

void runDynVectorTests()
{
    std::cout << subtestPretext << "Testing dyn vector\n";
    ahl::dyn::vector<int> v;
    assert(v.empty());
    assert(v.capacity() == 0);
    assert(v.size() == 0);
    v.reserve(25);
    assert(v.capacity() == 25);
    assert(v.empty());
    for(size_t i = 0; i < 25; ++i)
    {
        v.emplace_back() = i;
    }
    assert(v.size() == 25);
    v.push_back(25);
    for(size_t i = 0; i < 25; ++i)
    {
        assert(v[i] + 1 == v[i + 1]);
    }
    assert(v.capacity() == 32);
    v.pop_back();
    assert(v.back() == 24);
    assert(v.front() == 0);
    assert(v.at(5) == 5);
    v.resize(25);
    assert(v.back() == 24);
    v.resize(30, 5);
    assert(v.back() == 5);
    for(size_t i = 25; i < 30; ++i)
    {
        assert(v[i] == 5);
    }
    v.resize(24);
    assert(v.back() == 23);
    v.clear();
    assert(v.empty());
    assert(v.capacity() == 32);
    v.push_back(4);
    v.push_back(2);
    ahl::dyn::vector<int> v2;
    v2.push_back(1);
    assert(v2.capacity() == 8);
    assert(v2 != v);
    assert(v2 == v2);
    assert(v == v);
    v2 = v;
    assert(v2 == v);
    assert(v2.data() != v.data());
    ahl::dyn::vector<int> v3;
    v3 = std::move(v);
    assert(v3 == v2);
    assert(v.empty());
    assert(v.data() == nullptr);

    for(int i = 0; i < 5; ++i)
    {
        v.push_back(i);
    }
    v2 = v;

    assert(v.size() == 5);
    v.erase(2);
    v.erase(0);
    v.erase(2);
    assert(v.size() == 2);
    assert(v[0] == 1 && v[1] == 3);
    v.insert(0, 0);
    v.insert(3, 4);
    v.insert(2, 2);
    assert(v == v2);
}

void runConsistentVectorTests()
{
    std::cout << subtestPretext << "Testing consistent vector\n";
    ahl::dyn::consistent_vector<int> v;
    std::vector<int*> addresses;
    assert(v.capacity() == 0);
    assert(v.size() == 0);
    assert(v.empty());

    for(int i = 0; i < 1024; i++)
    {
        if((i & 1) == 1)
        {
            int& val = v.emplace_back();
            val = i;
            addresses.push_back(&val);
        }
        else
        {
            v.push_back(i);
            addresses.push_back(&(v.at(v.size() - 1)));
        }
    }

    assert(v.capacity() == 1024);
    assert(v.size() == 1024);
    assert(!v.empty());

    ahl::dyn::consistent_vector<int> v2 = v;

    for(int i = 0; i < 1024; i++)
    {
        assert(v[i] == i);
        assert(v2[i] == v[i]);
        assert(&v[i] == addresses[i]);
        assert(&v2[i] != addresses[i]);
    }

    ahl::dyn::consistent_vector<int> v3 = std::move(v);
    assert(v.empty());
    assert(v3 == v2);
    for(int i = 0; i < 1024; i++)
    {
        assert(&v3[i] == addresses[i]);
    }

    v2.clear();
    assert(v2.empty());
    assert(v2.capacity() != 0);
    // TODO maybe should test mem leaks??
}

void runBitVectorTests()
{
    std::cout << subtestPretext << "Testing bit vector\n";

    ahl::stack_bitvector<32> bitset1;
    assert(bitset1.size() == 0);
    assert(bitset1.capacity() == 32);
    assert(bitset1.data_capacity() == 1);

    bitset1.resize(6);
    assert(bitset1.size() == 6);
    assert(bitset1.capacity() == 32);
    assert(bitset1.data_capacity() == 1);

    ahl::stack_bitvector<128> bitset2;
    assert(bitset2.capacity() == 128);
    for(int i = 0; i < 6; i++)
    {
        bitset2.push_back(false);
    }
    assert(bitset1 == bitset2);

    bitset2.set_bit(3, true);
    assert(bitset1 != bitset2);

    bitset2.resize(120);
    bitset2.set_bit(0, true);
    bitset2.set_bit(100, true);
    ahl::stack_bitvector<5 * 33> bitset3 = bitset2;
    assert(bitset3 == bitset2);
    bitset3.set_bit(110, true);
    bitset3.set_bit(24, true);
    assert((bitset3 & bitset2) == bitset2);
    assert((bitset2 | bitset3) == bitset3);
    ahl::stack_bitvector<5 * 32 + 7> bitset4;
    bitset4.resize(bitset3.size());
    bitset4.set_bit(110, true);
    bitset4.set_bit(24, true);
    assert((bitset3 ^ bitset2) == bitset4);

    bitset4 = bitset4;
    assert(bitset4 == (bitset3 ^ bitset2));

    ahl::stack_bitvector<4 * 32 + 3> bitset5;
    bitset5 = bitset3;
    bitset5 |= bitset2;
    assert(bitset5 == bitset3);
    bitset5 = bitset3;
    bitset5 &= bitset2;
    assert(bitset5 == bitset2);
    bitset5 = bitset3;
    bitset5 ^= bitset2;
    assert(bitset5 == bitset4);

    bitset5.set_all_false();
    assert(bitset5.none_set());
    assert(!bitset5.any_set());
    assert(!bitset5.all_set());

    bitset5.set_bit(105, true);
    assert(bitset5[105] == true);
    assert(bitset5.any_set());
    assert(!bitset5.none_set());
    assert(!bitset5.all_set());

    bitset5.set_bit(105, false);
    assert(bitset5.none_set());

    bitset5.set_all_true();
    assert(bitset5.all_set());
    assert(bitset5.any_set());

    assert((~bitset5).none_set());
    assert(((~bitset3) & bitset3).none_set());
    assert(((~bitset3) | bitset3).all_set());
    assert(((~bitset3) ^ bitset3).all_set());
}

void runSTLTests()
{
    std::cout << "\n***********************************\nRunning Tests For AncestralHall...\n";
    runStackMaskedArrayTests();
    runFixedVectorTests();
    runStackSetTests();
    runStackMapTests();
    runDynVectorTests();
    runConsistentVectorTests();
    runBitVectorTests();
}