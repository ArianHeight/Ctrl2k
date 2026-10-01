#include "pch.h"

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
        unsigned int id = newVal.get_id();
        v2.push_back(std::move(newVal));
        EXPECT_EQ(v2.back().val, i);
        EXPECT_EQ(v2.back().get_id(), id);
        EXPECT_NE(newVal.get_id(), id);
    }

    ahl::stack_vector<int, 4> v3;
    for(int i = 0; i < 4; ++i)
    {
        v3.push_back(i);
    }
    EXPECT_DEBUG_DEATH(v3.push_back(4), ANY_ASSERT_REGEX);
}