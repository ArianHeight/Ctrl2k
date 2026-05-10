#include "tests.h"

#include <iostream>

#include "Core/AncestralHall/ahl.h"

void runFixedVectorTests()
{
    std::cout << subtestPretext << "Testing fixed vector of size 8\n";
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
}

void runConsistentVectorTests()
{
    std::cout << subtestPretext << "Testing consistent vector\n";
    ahl::dyn::consistent_vector<int> v;
    std::vector<int*> addresses;
    assert(v.capacity() == ahl::dyn::CONSISTENT_VECTOR_INITIAL_CAPACITY);
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

    assert(v.capacity() == 2016);
    assert(v.size() == 1024);
    assert(!v.empty());

    for(int i = 0; i < 1024; i++)
    {
        assert(v[i] == i);
        assert(&v[i] == addresses[i]);
    }

    // TODO maybe should test mem leaks??
}

void runBitVectorTests()
{
    std::cout << subtestPretext << "Testing bit vector\n";

    ahl::bit_vector<1> bitset1;
    assert(bitset1.size() == 0);
    assert(bitset1.capacity() == 32);
    assert(bitset1.data_capacity() == 1);

    bitset1.resize(6);
    assert(bitset1.size() == 6);
    assert(bitset1.capacity() == 32);
    assert(bitset1.data_capacity() == 1);

    ahl::bit_vector<4> bitset2;
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
    ahl::bit_vector<5> bitset3 = bitset2;
    assert(bitset3 == bitset2);
    bitset3.set_bit(110, true);
    bitset3.set_bit(24, true);
    assert((bitset3 & bitset2) == bitset2);
    assert((bitset2 | bitset3) == bitset3);
    ahl::bit_vector<5> bitset4;
    bitset4.resize(bitset3.size());
    bitset4.set_bit(110, true);
    bitset4.set_bit(24, true);
    assert((bitset3 ^ bitset2) == bitset4);

    ahl::bit_vector<4> bitset5;
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
    runFixedVectorTests();
    runConsistentVectorTests();
    runBitVectorTests();
}