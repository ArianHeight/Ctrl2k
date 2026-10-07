#include "pch.h"

static void genRandomTestDequeue(ahl::stack_dequeue<MoveTester, 32>& base, ahl::stack_dequeue<MoveTester::id_t, 32>& ids, int count)
{
    for(int i = 0; i < 32 && i < count; ++i)
    {
        MoveTester val = genRandInt();
        if(val.val & 1)
        {
            ids.push_back(val.get_id());
            base.push_back(std::move(val));
        }
        else
        {
            ids.push_front(val.get_id());
            base.push_front(std::move(val));
        }
    }
}


TEST(stack_dequeue, construction)
{
    ahl::stack_dequeue<MoveTester, 32> qbase;
    ahl::stack_dequeue<MoveTester::id_t, 32> qId;
    genRandomTestDequeue(qbase, qId, 16);

    ahl::stack_dequeue<MoveTester, 32> q1(qbase);
    EXPECT_EQ(q1, qbase);
    for(int i = 0; i < 16; ++i)
    {
        EXPECT_NE(q1[i].get_id(), qbase[i].get_id());
        EXPECT_EQ(qId[i], qbase[i].get_id());
    }

    ahl::stack_dequeue<MoveTester, 32> q2(std::move(qbase));
    EXPECT_EQ(q1, q2);
    EXPECT_TRUE(qbase.empty());
    for(int i = 0; i < 16; ++i)
    {
        EXPECT_NE(q2[i].get_id(), q1[i].get_id());
        EXPECT_EQ(qId[i], q2[i].get_id());
    }
}

TEST(stack_dequeue, assignment)
{
    ahl::stack_dequeue<MoveTester, 32> qbase;
    ahl::stack_dequeue<MoveTester::id_t, 32> qId;
    genRandomTestDequeue(qbase, qId, 16);

    ahl::stack_dequeue<MoveTester, 32> q1;
    q1.push_back(16);
    q1 = qbase;
    EXPECT_EQ(q1, qbase);
    for(int i = 0; i < 16; ++i)
    {
        EXPECT_NE(q1[i].get_id(), qbase[i].get_id());
        EXPECT_EQ(qId[i], qbase[i].get_id());
    }

    ahl::stack_dequeue<MoveTester, 32> q2;
    q2.push_back(13);
    q2 = std::move(qbase);
    EXPECT_EQ(q1, q2);
    EXPECT_TRUE(qbase.empty());
    for(int i = 0; i < 16; ++i)
    {
        EXPECT_NE(q2[i].get_id(), q1[i].get_id());
        EXPECT_EQ(qId[i], q2[i].get_id());
    }
}

TEST(stack_dequeue, template_construction)
{
    ahl::stack_dequeue<MoveTester, 32> qbase;
    ahl::stack_dequeue<MoveTester::id_t, 32> qId;
    genRandomTestDequeue(qbase, qId, 16);

    ahl::stack_dequeue<MoveTester, 16> q1(qbase);
    EXPECT_EQ(q1, qbase);
    for(int i = 0; i < 16; ++i)
    {
        EXPECT_NE(q1[i].get_id(), qbase[i].get_id());
        EXPECT_EQ(qId[i], qbase[i].get_id());
    }

    ahl::stack_dequeue<MoveTester, 16> q2(std::move(qbase));
    EXPECT_EQ(q1, q2);
    EXPECT_TRUE(qbase.empty());
    for(int i = 0; i < 16; ++i)
    {
        EXPECT_NE(q2[i].get_id(), q1[i].get_id());
        EXPECT_EQ(qId[i], q2[i].get_id());
    }
}

TEST(stack_dequeue, template_assignment)
{
    ahl::stack_dequeue<MoveTester, 32> qbase;
    ahl::stack_dequeue<MoveTester::id_t, 32> qId;
    genRandomTestDequeue(qbase, qId, 16);

    ahl::stack_dequeue<MoveTester, 16> q1;
    q1.push_back(14);
    q1 = qbase;
    EXPECT_EQ(q1, qbase);
    for(int i = 0; i < 16; ++i)
    {
        EXPECT_NE(q1[i].get_id(), qbase[i].get_id());
        EXPECT_EQ(qId[i], qbase[i].get_id());
    }

    ahl::stack_dequeue<MoveTester, 16> q2;
    q2.push_back(15);
    q2 = std::move(qbase);
    EXPECT_EQ(q1, q2);
    EXPECT_TRUE(qbase.empty());
    for(int i = 0; i < 16; ++i)
    {
        EXPECT_NE(q2[i].get_id(), q1[i].get_id());
        EXPECT_EQ(qId[i], q2[i].get_id());
    }
}

TEST(stack_dequeue, failed_cstr_assign)
{
    ahl::stack_dequeue<MoveTester, 32> qbase;
    ahl::stack_dequeue<MoveTester::id_t, 32> qId;
    genRandomTestDequeue(qbase, qId, 16);

    auto vCopy = qbase;
    qbase = qbase;
    EXPECT_EQ(vCopy, qbase);
    for(size_t i = 0; i < qbase.size(); ++i)
    {
        EXPECT_EQ(qbase[i].get_id(), qId[i]);
    }
    qbase = std::move(qbase);
    EXPECT_EQ(vCopy, qbase);
    for(size_t i = 0; i < qbase.size(); ++i)
    {
        EXPECT_EQ(qbase[i].get_id(), qId[i]);
    }

    auto copy_construct = [](const ahl::stack_dequeue<MoveTester, 32>& base)
    {
        ahl::stack_dequeue<MoveTester, 8> q1(base);
    };

    auto copy_assign = [](const ahl::stack_dequeue<MoveTester, 32>& base)
    {
        ahl::stack_dequeue<MoveTester, 8> q1;
        q1.push_back(32);
        q1 = base;
    };

    auto move_construct = [](ahl::stack_dequeue<MoveTester, 32>&& base)
    {
        ahl::stack_dequeue<MoveTester, 8> q2(std::move(base));
    };

    auto move_assign = [](ahl::stack_dequeue<MoveTester, 32>&& base)
    {
        ahl::stack_dequeue<MoveTester, 8> q2;
        q2.push_back(17);
        q2 = std::move(base);
    };

    EXPECT_DEBUG_DEATH(copy_construct(qbase), ANY_ASSERT_REGEX);
    EXPECT_DEBUG_DEATH(copy_assign(qbase), ANY_ASSERT_REGEX);
    EXPECT_DEBUG_DEATH(move_construct(std::move(qbase)), ANY_ASSERT_REGEX);
    EXPECT_DEBUG_DEATH(move_assign(std::move(qbase)), ANY_ASSERT_REGEX);
}

TEST(stack_dequeue, equals)
{
    ahl::stack_dequeue<MoveTester, 32> qbase;
    ahl::stack_dequeue<MoveTester::id_t, 32> ids;
    genRandomTestDequeue(qbase, ids, 16);

    ahl::stack_dequeue<MoveTester, 32> q1 = qbase;
    EXPECT_EQ(q1, qbase);

    ahl::stack_dequeue<MoveTester, 32> q2 = std::move(q1);
    EXPECT_EQ(q2, qbase);

    ahl::stack_dequeue<MoveTester, 16> q3 = qbase;
    EXPECT_EQ(q3, qbase);

    ahl::stack_dequeue<MoveTester, 16> q4 = std::move(q3);
    EXPECT_EQ(q4, qbase);

    int randomIndex = genRandInt(0, 15);
    ++q2[randomIndex].val;
    EXPECT_NE(q2, qbase);

    randomIndex = genRandInt(0, 15);
    ++q4[randomIndex].val;
    EXPECT_NE(q4, qbase);
}

TEST(stack_dequeue, push_pop)
{
    ahl::stack_dequeue<int, 16> q1;
    EXPECT_TRUE(q1.empty());

    for(int i = 0; i < 16; ++i)
    {
        if(i < 8)
        {
            q1.push_back(i);
        }
        else
        {
            q1.emplace_back() = i;
        }
    }
    for(int i = 0; i < 16; ++i)
    {
        EXPECT_EQ(q1.peek_front(), i);
        q1.pop_front();
    }

    for(int i = 0; i < 4; ++i)
    {
        if(i < 2)
        {
            q1.push_back(i);
        }
        else
        {
            q1.emplace_back() = i;
        }
    }
    for(int i = 0; i < 4; ++i)
    {
        EXPECT_EQ(q1.peek_front(), i);
        q1.pop_front();
    }

    for(int i = 0; i < 16; ++i)
    {
        if(i < 8)
        {
            q1.push_front(i);
        }
        else
        {
            q1.emplace_front() = i;
        }
    }
    for(int i = 0; i < 16; ++i)
    {
        EXPECT_EQ(q1.peek_back(), i);
        q1.pop_back();
    }

    EXPECT_TRUE(q1.empty());
    for(int i = 0; i < 8; ++i)
    {
        q1.push_back(i);
        q1.push_front(i);
    }
    for(int i = 0; i < 8; ++i)
    {
        EXPECT_EQ(q1.peek_back(), 7 - i);
        q1.pop_back();
    }
    for(int i = 0; i < 8; ++i)
    {
        EXPECT_EQ(q1.peek_back(), i);
        q1.pop_back();
    }
}

TEST(stack_dequeue, move_push)
{
    ahl::stack_dequeue<MoveTester, 16> q1;
    ahl::stack_vector<MoveTester::id_t, 16> ids;
    ahl::stack_vector<MoveTester, 16> data;

    for(int i = 0; i < 8; ++i)
    {
        data.push_back(i);
        ids.push_back(data.back().get_id());
    }
    for(int i = 0; i < 8; ++i)
    {
        if(i < 4)
        {
            q1.push_back(std::move(data[i]));
        }
        else
        {
            q1.push_front(std::move(data[i]));
        }
    }

    EXPECT_FALSE(q1.empty());
    EXPECT_EQ(q1.size(), 8);
    for(int i = 0; i < 4; ++i)
    {
        EXPECT_EQ(q1[i + 4], i);
        EXPECT_EQ(q1[i + 4].get_id(), ids[i]);
        EXPECT_EQ(q1[3 - i], i + 4);
        EXPECT_EQ(q1[3 - i].get_id(), ids[i + 4]);
    }
}

TEST(stack_dequeue, failed_push_pop)
{
    {
        ahl::stack_dequeue<int, 16> q0;
        for(int i = 0; i < 16; ++i)
        {
            q0.push_back(i);
        }
        EXPECT_DEBUG_DEATH(q0.push_front(17), ANY_ASSERT_REGEX);
    }
    {
        ahl::stack_dequeue<int, 16> q1;
        for(int i = 0; i < 16; ++i)
        {
            q1.push_front(i);
        }
        EXPECT_DEBUG_DEATH(q1.push_back(17), ANY_ASSERT_REGEX);
    }
    {
        ahl::stack_dequeue<MoveTester, 16> q0m;
        for(int i = 0; i < 16; ++i)
        {
            q0m.push_back(i);
        }
        EXPECT_DEBUG_DEATH(q0m.push_front(17), ANY_ASSERT_REGEX);
    }
    {
        ahl::stack_dequeue<MoveTester, 16> q1m;
        for(int i = 0; i < 16; ++i)
        {
            q1m.push_front(i);
        }
        EXPECT_DEBUG_DEATH(q1m.push_back(17), ANY_ASSERT_REGEX);
    }
    {
        ahl::stack_dequeue<int, 16> q2;
        EXPECT_DEBUG_DEATH(q2.pop_front(), ANY_ASSERT_REGEX);
    }
    {
        ahl::stack_dequeue<int, 16> q3;
        EXPECT_DEBUG_DEATH(q3.peek_front(), ANY_ASSERT_REGEX);
    }
    {
        ahl::stack_dequeue<int, 16> q4;
        EXPECT_DEBUG_DEATH(q4.pop_back(), ANY_ASSERT_REGEX);
    }
    {
        ahl::stack_dequeue<int, 16> q5;
        EXPECT_DEBUG_DEATH(q5.peek_back(), ANY_ASSERT_REGEX);
    }
}

TEST(stack_dequeue, misc)
{
    ahl::stack_dequeue<int, 32> q1;

    EXPECT_TRUE(q1.empty());
    EXPECT_EQ(q1.capacity(), 32);
    EXPECT_EQ(q1.size(), 0);

    for(int i = 0; i < 30; ++i)
    {
        q1.push_front(i);
    }

    EXPECT_FALSE(q1.empty());
    EXPECT_EQ(q1.size(), 30);
    EXPECT_EQ(q1.capacity(), 32);

    q1.clear();
    EXPECT_TRUE(q1.empty());
    EXPECT_EQ(q1.size(), 0);
    EXPECT_EQ(q1.capacity(), 32);

    for(int i = 0; i < 30; ++i)
    {
        q1.push_front(i);
    }
    for(int i = 0; i < 30; ++i)
    {
        EXPECT_EQ(q1[29 - i], i);
    }
}

TEST(stack_dequeue, access)
{
    ahl::stack_vector<int, 16> vbase;
    for(int i = 0; i < 16; ++i)
    {
        vbase.push_back(genRandInt());
    }

    ahl::stack_dequeue<int, 16> q1;
    for(int i = 0; i < 16; ++i)
    {
        q1.push_back(vbase[i]);
    }
    for(int i = 0; i < 16; ++i)
    {
        if(i & 1)
        {
            EXPECT_EQ(q1.at(i), vbase[i]);
        }
        else
        {
            EXPECT_EQ(q1[i], vbase[i]);
        }
    }

    ahl::stack_dequeue<int, 16> q2;
    for(int i = 0; i < 16; ++i)
    {
        q2.push_front(vbase[i]);
    }
    for(int i = 0; i < 16; ++i)
    {
        if(i & 1)
        {
            EXPECT_EQ(q2.at(15 - i), vbase[i]);
        }
        else
        {
            EXPECT_EQ(q2[15 - i], vbase[i]);
        }
    }
}
