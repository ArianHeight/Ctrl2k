#include "tests.h"

#include <iostream>

#include "Core/AncestralHall/fixed_vector.h"

void runFixedVectorTests()
{
    std::cout << subtestPretext << "Testing fixed vector of size 8\n";
    ahl::fixed_vector<int, 8> v1;
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

void runSTLTests()
{
    std::cout << "\n***********************************\nRunning Tests For AncestralHall...\n";
    runFixedVectorTests();
}