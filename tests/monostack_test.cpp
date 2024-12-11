#include <gtest/gtest.h>
#include <stdexcept>
#include "../src/stack/my_mono_stack.hpp"

class MonotonicStackTest : public testing::Test {
protected:
    MonotonicStack<int> m_stackInc;
    MonotonicStack<int, std::greater<>> m_stackDec;
};

TEST_F(MonotonicStackTest, PushMaintainsMonotonicInc) {
    m_stackInc.push(3);
    m_stackInc.push(2);
    m_stackInc.push(5);

    EXPECT_EQ(m_stackInc.size(), 2);
    EXPECT_EQ(m_stackInc.top(), 5);
}

TEST_F(MonotonicStackTest, PushMaintainsMonotonicDec) {
    m_stackDec.push(3);
    m_stackDec.push(4);
    m_stackDec.push(2);

    EXPECT_EQ(m_stackDec.size(), 2);
    EXPECT_EQ(m_stackDec.top(), 2);
}

TEST_F(MonotonicStackTest, PopReducesStackSize) {
    m_stackInc.push(1);
    m_stackInc.push(3);
    m_stackInc.push(2);

    EXPECT_EQ(m_stackInc.top(), 2);
    m_stackInc.pop();
    EXPECT_EQ(m_stackInc.top(), 1);
    m_stackInc.pop();
    EXPECT_EQ(m_stackInc.size(), 0);
}

TEST_F(MonotonicStackTest, ClearEmptiesStack) {
    m_stackInc.push(10);
    m_stackInc.push(20);

    EXPECT_EQ(m_stackInc.size(), 2);
    m_stackInc.clear();
    EXPECT_EQ(m_stackInc.size(), 0);
    EXPECT_TRUE(m_stackInc.empty());
}

TEST_F(MonotonicStackTest, TopThrowsOnEmptyStack) { EXPECT_THROW(m_stackInc.top(), std::out_of_range); }

TEST_F(MonotonicStackTest, EmptyStackBehavior) {
    EXPECT_TRUE(m_stackInc.empty());
    m_stackInc.push(7);
    EXPECT_FALSE(m_stackInc.empty());
}

TEST_F(MonotonicStackTest, HandlesLargeInput) {
    for (int i = 0; i < 1000; ++i) {
        m_stackInc.push(i);
    }

    EXPECT_EQ(m_stackInc.size(), 1000);
    for (int i = 999; i >= 0; --i) {
        EXPECT_EQ(m_stackInc.top(), i);
        m_stackInc.pop();
    }

    EXPECT_TRUE(m_stackInc.empty());
}
