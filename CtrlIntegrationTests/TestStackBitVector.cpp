#include "pch.h"

static void genRandomTestBitVector(ahl::stack_bitvector<44>& v, int count)
{
    v.resize(count);
    const int numFlippedBits = count / 4;
    for(int i = 0; i < numFlippedBits; ++i)
    {
        v.set_bit(genRandInt(0, count - 1), true);
    }
}

TEST(stack_bitvector, construction)
{
    ahl::stack_bitvector<44> vbase;
    genRandomTestBitVector(vbase, 40);

    ahl::stack_bitvector<44> v1 = vbase;
    EXPECT_EQ(v1, vbase);

    ahl::stack_bitvector<48> v2 = vbase;
    EXPECT_EQ(v2, vbase);

    ahl::stack_bitvector<40> v3 = vbase;
    EXPECT_EQ(v3, vbase);
}

TEST(stack_bitvector, assignment)
{
    ahl::stack_bitvector<44> vbase;
    genRandomTestBitVector(vbase, 40);

    ahl::stack_bitvector<44> v1;
    v1.push_back(true);
    v1 = vbase;
    EXPECT_EQ(v1, vbase);

    ahl::stack_bitvector<48> v2;
    v2.push_back(true);
    v2 = vbase;
    EXPECT_EQ(v2, vbase);

    ahl::stack_bitvector<40> v3;
    v3.push_back(true);
    v3 = vbase;
    EXPECT_EQ(v3, vbase);
}

TEST(stack_bitvector, failed_cstr_assign)
{
    ahl::stack_bitvector<44> vbase;
    genRandomTestBitVector(vbase, 40);

    auto copyConstruct = [](const ahl::stack_bitvector<44> base)
    {
        ahl::stack_bitvector<34> v1 = base;
    };

    auto copyAssign = [](const ahl::stack_bitvector<44> base)
    {
        ahl::stack_bitvector<34> v2;
        v2.push_back(true);
        v2 = base;
    };

    EXPECT_DEBUG_DEATH(copyConstruct(vbase), ANY_ASSERT_REGEX);
    EXPECT_DEBUG_DEATH(copyAssign(vbase), ANY_ASSERT_REGEX);
}

TEST(stack_bitvector, equals)
{
    ahl::stack_bitvector<44> vbase;
    genRandomTestBitVector(vbase, 44);

    ahl::stack_bitvector<44> v1 = vbase;
    int randIndex = genRandInt(0, 43);
    EXPECT_EQ(v1, vbase);
    v1.flip_bit(randIndex);
    EXPECT_NE(v1, vbase);

    ahl::stack_bitvector<44> v2;
    genRandomTestBitVector(v2, 44);
    randIndex = genRandInt(0, 43);
    v2.set_bit(randIndex, !vbase.get_bit(randIndex));
    EXPECT_NE(v2, vbase);
}

TEST(stack_bitvector, binary)
{
    ahl::stack_bitvector<44> vbase;
    genRandomTestBitVector(vbase, 39);

    ahl::stack_bitvector<46> v1 = ~vbase;
    for(int i = 0; i < 39; ++i)
    {
        EXPECT_NE(v1[i], vbase[i]);
    }

    ahl::stack_bitvector<42> vTrue;
    vTrue.resize(39, true);
    ahl::stack_bitvector<43> vFalse;
    vFalse.resize(39, false);
    ahl::stack_bitvector<47> v2 = vbase ^ vTrue;
    EXPECT_EQ(v2, v1);
    v2 = vbase ^ ~vbase;
    EXPECT_EQ(v2, vTrue);
    v2 = vbase ^ vbase;
    EXPECT_EQ(v2, vFalse);

    v2 = vbase;
    v2 ^= vTrue;
    EXPECT_EQ(v1, v2);
    v2 ^= vbase;
    EXPECT_EQ(v2, vTrue);
    v2 = vbase;
    v2 ^= vbase;
    EXPECT_EQ(v2, vFalse);

    v1 = vbase & vTrue;
    EXPECT_EQ(v1, vbase);
    v1 = vbase & vFalse;
    EXPECT_EQ(v1, vFalse);
    v1 = vbase & ~vbase;
    EXPECT_EQ(v1, vFalse);

    v2 = vbase;
    v2 &= vTrue;
    EXPECT_EQ(v2, vbase);
    v2 = vbase;
    v2 &= vFalse;
    EXPECT_EQ(v2, vFalse);
    v2 = vbase;
    v2 &= ~vbase;
    EXPECT_EQ(v2, vFalse);

    v1 = vbase | vTrue;
    EXPECT_EQ(v1, vTrue);
    v2 = vbase | vFalse;
    EXPECT_EQ(v2, vbase);
    v1 = vbase | vbase;
    EXPECT_EQ(v1, v2);
    v2 = v1 | ~vbase;
    EXPECT_EQ(v2, vTrue);

    v1 = vbase;
    v1 |= vTrue;
    EXPECT_EQ(v1, vTrue);
    v2 = vbase;
    v2 |= vFalse;
    EXPECT_EQ(v2, vbase);
    v2 |= vbase;
    EXPECT_EQ(v2, vbase);
    v2 |= ~vbase;
    EXPECT_EQ(v2, vTrue);
}

TEST(stack_bitvector, misc)
{
    ahl::stack_bitvector<40> v1;
    v1.resize(30);
    EXPECT_FALSE(v1.empty());
    EXPECT_EQ(v1.size(), 30);
    EXPECT_EQ(v1.data_capacity(), 2);
    EXPECT_EQ(v1.capacity(), 40);
    v1.clear();
    EXPECT_TRUE(v1.empty());
    EXPECT_EQ(v1.size(), 0);
    EXPECT_EQ(v1.data_capacity(), 2);
    EXPECT_EQ(v1.capacity(), 40);
}

TEST(stack_bitvector, push_back)
{
    ahl::stack_bitvector<40> v1;
    EXPECT_TRUE(v1.empty());
    EXPECT_EQ(v1.size(), 0);

    for(int i = 0; i < 35; ++i)
    {
        v1.push_back((i & 1) != 0);
    }
    EXPECT_EQ(v1.size(), 35);
    EXPECT_FALSE(v1.all_set());
    EXPECT_TRUE(v1.any_set());

    for(int i = 0; i < 35; ++i)
    {
        EXPECT_EQ(v1[i], (i & 1) != 0);
    }

    v1.resize(40);
    EXPECT_DEBUG_DEATH(v1.push_back(true), ANY_ASSERT_REGEX);
}

TEST(stack_bitvector, set)
{
    ahl::stack_bitvector<42> v1;

    v1.resize(42);
    EXPECT_TRUE(v1.none_set());

    v1.set_all_true();
    EXPECT_TRUE(v1.any_set());
    EXPECT_TRUE(v1.all_set());
    
    v1.set_all_false();
    EXPECT_TRUE(v1.none_set());
    EXPECT_FALSE(v1.all_set());
    EXPECT_FALSE(v1.any_set());

    for(int i = 0; i < 42; ++i)
    {
        EXPECT_FALSE(v1.get_bit(i));
        v1.set_bit(i, true);
        EXPECT_FALSE(v1.all_set());
        EXPECT_FALSE(v1.none_set());
        EXPECT_TRUE(v1.any_set());
        EXPECT_TRUE(v1[i]);
        v1.set_all_false();
        EXPECT_FALSE(v1.at(i));
    }

    v1.set_all_true();
    for(int i = 0; i < 42; ++i)
    {
        EXPECT_TRUE(v1[i]);
        v1.set_bit(i, false);
        EXPECT_FALSE(v1.all_set());
        EXPECT_FALSE(v1.none_set());
        EXPECT_TRUE(v1.any_set());
        EXPECT_FALSE(v1[i]);
        v1.set_all_true();
        EXPECT_TRUE(v1[i]);
    }
}

TEST(stack_bitvector, flip)
{
    ahl::stack_bitvector<50> v1;
    
    v1.resize(40);
    int randIndex = genRandInt(0, 49);
    v1.flip_bit(randIndex);
    for(int i = 0; i < 40; ++i)
    {
        EXPECT_EQ(v1[i], i == randIndex);
    }

    v1.flip_all_bits();
    for(int i = 0; i < 40; ++i)
    {
        EXPECT_EQ(v1[i], i != randIndex);
    }
}

TEST(stack_bitvector, stack)
{
    ahl::stack_bitvector<43> v1;
    v1.resize(35);
    EXPECT_FALSE(v1.front());
    v1.set_bit(0, true);
    EXPECT_TRUE(v1.front());
    v1.push_back(true);
    EXPECT_TRUE(v1.back());
    v1.push_back(false);
    EXPECT_FALSE(v1.back());
    v1.pop_back();
    EXPECT_TRUE(v1.back());
    v1.pop_back();
    EXPECT_FALSE(v1.back());
    v1.pop_back();
    EXPECT_FALSE(v1.back());
}

TEST(stack_bitvector, resize)
{
    ahl::stack_bitvector<51> v1;
    v1.resize(36);
    v1.resize(40, true);
    v1.resize(48);

    for(int i = 0; i < 48; ++i)
    {
        EXPECT_EQ(v1[i], i >= 36 && i < 40);
    }

    v1.resize(26);
    v1.resize(35, true);
    for(int i = 0; i < 35; ++i)
    {
        EXPECT_EQ(v1[i], i >= 26);
    }

    v1.clear();
    v1.resize(5, true);
    for(int i = 0; i < 5; ++i)
    {
        EXPECT_TRUE(v1[i]);
    }

    v1.resize(51);
    EXPECT_DEBUG_DEATH(v1.resize(52), ANY_ASSERT_REGEX);
}
