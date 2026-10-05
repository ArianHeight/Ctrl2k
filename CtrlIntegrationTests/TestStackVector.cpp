#include "pch.h"

static void genRandomTestVector(ahl::stack_vector<MoveTester, 16>& base, ahl::stack_vector<MoveTester::id_t, 16>& ids, int count)
{
    for(int i = 0; i < 12; ++i)
    {
        base.push_back(genRandInt());
        ids.push_back(base.back().get_id());
    }
}

TEST(stack_vector, construction)
{
    ahl::stack_vector<MoveTester, 16> vbase;
    ahl::stack_vector<MoveTester::id_t, 16> vId;
    genRandomTestVector(vbase, vId, 12);
    
    ahl::stack_vector<MoveTester, 16> v1(vbase);
    EXPECT_EQ(v1, vbase);
    for(int i = 0; i < 12; ++i)
    {
        EXPECT_NE(v1[i].get_id(), vbase[i].get_id());
        EXPECT_EQ(vId[i], vbase[i].get_id());
    }

    ahl::stack_vector<MoveTester, 16> v2(std::move(vbase));
    EXPECT_EQ(v1, v2);
    EXPECT_TRUE(vbase.empty());
    for(int i = 0; i < 12; ++i)
    {
        EXPECT_NE(v2[i].get_id(), v1[i].get_id());
        EXPECT_EQ(vId[i], v2[i].get_id());
    }
}

TEST(stack_vector, assignment)
{
    ahl::stack_vector<MoveTester, 16> vbase;
    ahl::stack_vector<MoveTester::id_t, 16> vId;
    genRandomTestVector(vbase, vId, 12);

    ahl::stack_vector<MoveTester, 16> v1;
    v1.push_back(12);
    v1 = vbase;
    EXPECT_EQ(v1, vbase);
    for(int i = 0; i < 12; ++i)
    {
        EXPECT_NE(v1[i].get_id(), vbase[i].get_id());
        EXPECT_EQ(vId[i], vbase[i].get_id());
    }

    ahl::stack_vector<MoveTester, 16> v2;
    v2.push_back(13);
    v2 = std::move(vbase);
    EXPECT_EQ(v1, v2);
    EXPECT_TRUE(vbase.empty());
    for(int i = 0; i < 12; ++i)
    {
        EXPECT_NE(v2[i].get_id(), v1[i].get_id());
        EXPECT_EQ(vId[i], v2[i].get_id());
    }
}

TEST(stack_vector, template_construction)
{
    ahl::stack_vector<MoveTester, 16> vbase;
    ahl::stack_vector<MoveTester::id_t, 16> vId;
    genRandomTestVector(vbase, vId, 12);

    ahl::stack_vector<MoveTester, 12> v1(vbase);
    EXPECT_EQ(v1, vbase);
    for(int i = 0; i < 12; ++i)
    {
        EXPECT_NE(v1[i].get_id(), vbase[i].get_id());
        EXPECT_EQ(vId[i], vbase[i].get_id());
    }

    ahl::stack_vector<MoveTester, 12> v2(std::move(vbase));
    EXPECT_EQ(v1, v2);
    EXPECT_TRUE(vbase.empty());
    for(int i = 0; i < 12; ++i)
    {
        EXPECT_NE(v2[i].get_id(), v1[i].get_id());
        EXPECT_EQ(vId[i], v2[i].get_id());
    }
}

TEST(stack_vector, template_assignment)
{
    ahl::stack_vector<MoveTester, 16> vbase;
    ahl::stack_vector<MoveTester::id_t, 16> vId;
    genRandomTestVector(vbase, vId, 12);

    ahl::stack_vector<MoveTester, 12> v1;
    v1.push_back(14);
    v1 = vbase;
    EXPECT_EQ(v1, vbase);
    for(int i = 0; i < 12; ++i)
    {
        EXPECT_NE(v1[i].get_id(), vbase[i].get_id());
        EXPECT_EQ(vId[i], vbase[i].get_id());
    }

    ahl::stack_vector<MoveTester, 12> v2;
    v2.push_back(15);
    v2 = std::move(vbase);
    EXPECT_EQ(v1, v2);
    EXPECT_TRUE(vbase.empty());
    for(int i = 0; i < 12; ++i)
    {
        EXPECT_NE(v2[i].get_id(), v1[i].get_id());
        EXPECT_EQ(vId[i], v2[i].get_id());
    }
}

TEST(stack_vector, failed_cstr_assign)
{
    ahl::stack_vector<MoveTester, 16> vbase;
    ahl::stack_vector<MoveTester::id_t, 16> vId;
    genRandomTestVector(vbase, vId, 12);

    auto vCopy = vbase;
    vbase = vbase;
    EXPECT_EQ(vCopy, vbase);
    for(size_t i = 0; i < vbase.size(); ++i)
    {
        EXPECT_EQ(vbase[i].get_id(), vId[i]);
    }
    vbase = std::move(vbase);
    EXPECT_EQ(vCopy, vbase);
    for(size_t i = 0; i < vbase.size(); ++i)
    {
        EXPECT_EQ(vbase[i].get_id(), vId[i]);
    }

    auto copy_construct = [](const ahl::stack_vector<MoveTester, 16>& base)
    {
        ahl::stack_vector<MoveTester, 8> v1(base);
    };

    auto copy_assign = [](const ahl::stack_vector<MoveTester, 16>& base)
    {
        ahl::stack_vector<MoveTester, 8> v1;
        v1.push_back(16);
        v1 = base;
    };

    auto move_construct = [](ahl::stack_vector<MoveTester, 16>&& base)
    {
        ahl::stack_vector<MoveTester, 8> v2(std::move(base));
    };

    auto move_assign = [](ahl::stack_vector<MoveTester, 16>&& base)
    {
        ahl::stack_vector<MoveTester, 8> v2;
        v2.push_back(17);
        v2 = std::move(base);
    };

    EXPECT_DEBUG_DEATH(copy_construct(vbase), ANY_ASSERT_REGEX);
    EXPECT_DEBUG_DEATH(copy_assign(vbase), ANY_ASSERT_REGEX);
    EXPECT_DEBUG_DEATH(move_construct(std::move(vbase)), ANY_ASSERT_REGEX);
    EXPECT_DEBUG_DEATH(move_assign(std::move(vbase)), ANY_ASSERT_REGEX);
}

TEST(stack_vector, equals)
{
    ahl::stack_vector<int, 16> vbase;
    for(int i = 0; i < 16; ++i)
    {
        vbase.push_back(genRandInt());
    }

    ahl::stack_vector<int, 16> v1 = vbase;
    int randIndex = genRandInt(0, 15);
    EXPECT_EQ(v1, vbase);
    v1[randIndex]++;
    EXPECT_NE(v1, vbase);

    ahl::stack_vector<int, 16> v2;
    for(int i = 0; i < 16; ++i)
    {
        v2.push_back(genRandInt());
    }

    randIndex = genRandInt(0, 15);
    v2[randIndex] = vbase[randIndex] + 1;
    EXPECT_NE(v2, vbase);
}

TEST(stack_vector, misc)
{
    ahl::stack_vector<MoveTester, 16> vbase;
    ahl::stack_vector<MoveTester::id_t, 16> vId;
    genRandomTestVector(vbase, vId, 12);

    EXPECT_EQ(vbase.size(), 12);
    EXPECT_FALSE(vbase.empty());
    auto v1 = vbase;
    vbase.clear();
    EXPECT_NE(v1, vbase);
    EXPECT_TRUE(vbase.empty());
    v1.clear();
    EXPECT_EQ(v1, vbase);
}

TEST(stack_vector, push_back)
{
    ahl::stack_vector<int, 16> v1;
    EXPECT_TRUE(v1.empty());
    EXPECT_EQ(v1.capacity(), 16);
    for(int i = 0; i < 12; ++i)
    {
        v1.push_back(i);
        EXPECT_EQ(v1.back(), i);
    }
    EXPECT_TRUE(!v1.empty());
    EXPECT_EQ(v1.size(), 12);

    ahl::stack_vector<MoveTester, 16> v2;
    for(int i = 0; i < 12; ++i)
    {
        MoveTester newVal = i;
        MoveTester::id_t id = newVal.get_id();
        v2.push_back(std::move(newVal));
        EXPECT_EQ(v2.back().val, i);
        EXPECT_EQ(v2.back().get_id(), id);
        EXPECT_NE(newVal.get_id(), id);
    }

    ahl::stack_vector<int, 4> v3;
    EXPECT_EQ(v3.capacity(), 4);
    for(int i = 0; i < 4; ++i)
    {
        v3.push_back(i);
    }
    EXPECT_DEBUG_DEATH(v3.push_back(4), ANY_ASSERT_REGEX);
}

TEST(stack_vector, emplace_back)
{
    ahl::stack_vector<int, 16> v1;
    EXPECT_TRUE(v1.empty());
    EXPECT_EQ(v1.capacity(), 16);
    for(int i = 0; i < 12; ++i)
    {
        auto& val = v1.emplace_back();
        val = i;
        EXPECT_EQ(v1.back(), i);
    }
    EXPECT_TRUE(!v1.empty());
    EXPECT_EQ(v1.size(), 12);

    ahl::stack_vector<MoveTester, 16> v2;
    for(int i = 0; i < 12; ++i)
    {
        MoveTester newVal = i;
        unsigned int id = newVal.get_id();
        auto& val = v2.emplace_back();
        val = std::move(newVal);
        EXPECT_EQ(v2.back().val, i);
        EXPECT_EQ(v2.back().get_id(), id);
        EXPECT_NE(newVal.get_id(), id);
    }

    ahl::stack_vector<int, 4> v3;
    EXPECT_EQ(v3.capacity(), 4);
    for(int i = 0; i < 4; ++i)
    {
        v3.emplace_back();
    }
    EXPECT_DEBUG_DEATH(v3.emplace_back(), ANY_ASSERT_REGEX);
}

TEST(stack_vector, access_data)
{
    ahl::stack_vector<int, 16> v1;
    for(int i = 0; i < 10; ++i)
    {
        v1.push_back(i);
    }

    for(int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(v1[i], i);
        EXPECT_EQ(v1.at(i), i);
        EXPECT_EQ(v1.data()[i], i);
    }

    for(int i = 0; i < 5; ++i)
    {
        v1[i] = 9 - i;
    }
    for(int i = 5; i < 10; ++i)
    {
        v1.at(i) = 9 - i;
    }

    for(int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(v1[i], 9 - i);
        EXPECT_EQ(v1.at(i), 9 - i);
        EXPECT_EQ(v1.data()[i], 9 - i);
    }

    EXPECT_DEBUG_DEATH(v1[10], ANY_ASSERT_REGEX);
}

TEST(stack_vector, stack)
{
    ahl::stack_vector<int, 16> v1;
    for(int i = 0; i < 10; ++i)
    {
        v1.push_back(i);
    }

    EXPECT_EQ(v1.size(), 10);
    for(int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(v1.front(), 0);
        EXPECT_EQ(v1.back(), 9 - i);
        v1.pop_back();
    }
    EXPECT_TRUE(v1.empty());
}

TEST(stack_vector, insertion)
{
    ahl::stack_vector<int, 16> v1;
    for(int i = 0; i < 10; ++i)
    {
        v1.push_back(i);
    }

    for(int i = 0; i < 6; ++i)
    {
        v1.insert(2 * i, i);
    }

    EXPECT_EQ(v1.size(), 16);

    for(int i = 0; i < 6; ++i)
    {
        EXPECT_EQ(v1[2 * i], i);
    }

    for(int i = 0; i < 6; ++i)
    {
        v1.erase((5 - i) * 2);
    }

    for(int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(v1[i], i);
    }

    v1.insert(10, 100);
    EXPECT_EQ(v1[10], 100);
    EXPECT_EQ(v1.size(), 11);
    v1.erase(10);
    EXPECT_EQ(v1.size(), 10);

    EXPECT_DEBUG_DEATH(v1.insert(11, 1000), ANY_ASSERT_REGEX);
    EXPECT_DEBUG_DEATH(v1.erase(12), ANY_ASSERT_REGEX);
}

TEST(stack_vector, resize)
{
    ahl::stack_vector<int, 16> v1;

    v1.resize(10, 5);
    EXPECT_EQ(v1.size(), 10);
    for(int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(v1[i], 5);
    }

    v1.resize(16, 12);
    EXPECT_EQ(v1.size(), 16);
    for(int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(v1[i], 5);
    }
    for(int i = 10; i < 16; ++i)
    {
        EXPECT_EQ(v1[i], 12);
    }

    v1.resize(3);
    EXPECT_EQ(v1.size(), 3);
    for(int i = 0; i < 3; ++i)
    {
        EXPECT_EQ(v1[i], 5);
    }
    
    v1.clear();
    v1.resize(12);
    EXPECT_EQ(v1.size(), 12);
    for(int i = 0; i < 12; ++i)
    {
        EXPECT_EQ(v1[i], int{});
    }

    EXPECT_DEBUG_DEATH(v1.resize(17), ANY_ASSERT_REGEX);
}
