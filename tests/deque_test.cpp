#include <gtest/gtest.h>
#include "../src/queue/my_deque.hpp"

class MyDequeTest : public ::testing::Test {
protected:
    Deque<int> m_deque;
};

TEST_F(MyDequeTest, EmptyDeque) {
    EXPECT_TRUE(m_deque.empty());
    EXPECT_EQ(m_deque.size(), 0);
    EXPECT_THROW(m_deque.front(), std::out_of_range);
    EXPECT_THROW(m_deque.back(), std::out_of_range);
    EXPECT_EQ(m_deque.at(0), std::nullopt);
}

TEST_F(MyDequeTest, PushFrontWorks) {
    m_deque.pushFront(10);
    EXPECT_FALSE(m_deque.empty());
    EXPECT_EQ(m_deque.size(), 1);
    EXPECT_EQ(m_deque.front(), 10);
    EXPECT_EQ(m_deque.back(), 10);

    m_deque.pushFront(20);
    EXPECT_EQ(m_deque.size(), 2);
    EXPECT_EQ(m_deque.front(), 20);
    EXPECT_EQ(m_deque.back(), 10);
}

TEST_F(MyDequeTest, PushBackWorks) {
    m_deque.pushBack(10);
    EXPECT_FALSE(m_deque.empty());
    EXPECT_EQ(m_deque.size(), 1);
    EXPECT_EQ(m_deque.front(), 10);
    EXPECT_EQ(m_deque.back(), 10);

    m_deque.pushBack(20);
    EXPECT_EQ(m_deque.size(), 2);
    EXPECT_EQ(m_deque.front(), 10);
    EXPECT_EQ(m_deque.back(), 20);
}

TEST_F(MyDequeTest, PopFrontWorks) {
    m_deque.pushBack(10);
    m_deque.pushBack(20);
    m_deque.pushBack(30);

    EXPECT_EQ(m_deque.size(), 3);
    EXPECT_EQ(m_deque.front(), 10);

    m_deque.popFront();
    EXPECT_EQ(m_deque.size(), 2);
    EXPECT_EQ(m_deque.front(), 20);

    m_deque.popFront();
    EXPECT_EQ(m_deque.size(), 1);
    EXPECT_EQ(m_deque.front(), 30);

    m_deque.popFront();
    EXPECT_TRUE(m_deque.empty());
    EXPECT_THROW(m_deque.front(), std::out_of_range);
}

TEST_F(MyDequeTest, PopBackWorks) {
    m_deque.pushBack(10);
    m_deque.pushBack(20);
    m_deque.pushBack(30);

    EXPECT_EQ(m_deque.size(), 3);
    EXPECT_EQ(m_deque.back(), 30);

    m_deque.popBack();
    EXPECT_EQ(m_deque.size(), 2);
    EXPECT_EQ(m_deque.back(), 20);

    m_deque.popBack();
    EXPECT_EQ(m_deque.size(), 1);
    EXPECT_EQ(m_deque.back(), 10);

    m_deque.popBack();
    EXPECT_TRUE(m_deque.empty());
    EXPECT_THROW(m_deque.back(), std::out_of_range);
}

TEST_F(MyDequeTest, AtWorksCorrectly) {
    m_deque.pushBack(10);
    m_deque.pushBack(20);
    m_deque.pushBack(30);

    EXPECT_EQ(m_deque.at(0), 10);
    EXPECT_EQ(m_deque.at(1), 20);
    EXPECT_EQ(m_deque.at(2), 30);
    EXPECT_EQ(m_deque.at(3), std::nullopt);
}

TEST_F(MyDequeTest, ClearWorks) {
    m_deque.pushBack(10);
    m_deque.pushBack(20);
    m_deque.pushBack(30);

    EXPECT_EQ(m_deque.size(), 3);
    m_deque.clear();
    EXPECT_TRUE(m_deque.empty());
    EXPECT_EQ(m_deque.size(), 0);
    EXPECT_THROW(m_deque.front(), std::out_of_range);
    EXPECT_THROW(m_deque.back(), std::out_of_range);
}

TEST_F(MyDequeTest, PushFrontAndPushBackTogether) {
    m_deque.pushFront(10);
    m_deque.pushBack(20);
    m_deque.pushFront(30);
    m_deque.pushBack(40);

    EXPECT_EQ(m_deque.size(), 4);
    EXPECT_EQ(m_deque.front(), 30);
    EXPECT_EQ(m_deque.back(), 40);
}

TEST_F(MyDequeTest, PopFrontAndPopBackTogether) {
    m_deque.pushBack(10);
    m_deque.pushBack(20);
    m_deque.pushBack(30);
    m_deque.pushBack(40);

    EXPECT_EQ(m_deque.size(), 4);

    m_deque.popFront();
    EXPECT_EQ(m_deque.front(), 20);
    EXPECT_EQ(m_deque.back(), 40);

    m_deque.popBack();
    EXPECT_EQ(m_deque.front(), 20);
    EXPECT_EQ(m_deque.back(), 30);

    m_deque.popFront();
    m_deque.popBack();

    EXPECT_TRUE(m_deque.empty());
    EXPECT_EQ(m_deque.size(), 0);
    EXPECT_THROW(m_deque.front(), std::out_of_range);
    EXPECT_THROW(m_deque.back(), std::out_of_range);
}
