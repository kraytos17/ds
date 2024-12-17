#include <gtest/gtest.h>
#include "../src/queue/my_queue.hpp"

class QueueTest : public testing::Test {
protected:
    Queue<int> m_queue;
};

TEST_F(QueueTest, EnqueueIncreasesSize) {
    m_queue.enqueue(1);
    m_queue.enqueue(2);
    m_queue.enqueue(3);

    EXPECT_EQ(m_queue.size(), 3);
}

TEST_F(QueueTest, FrontAndBackAreCorrect) {
    m_queue.enqueue(1);
    m_queue.enqueue(2);
    m_queue.enqueue(3);

    EXPECT_EQ(m_queue.front(), 1);
    EXPECT_EQ(m_queue.back(), 3);
}

TEST_F(QueueTest, DequeueDecreasesSize) {
    m_queue.enqueue(1);
    m_queue.enqueue(2);
    m_queue.enqueue(3);

    m_queue.dequeue();
    EXPECT_EQ(m_queue.size(), 2);
    EXPECT_EQ(m_queue.front(), 2);

    m_queue.dequeue();
    EXPECT_EQ(m_queue.size(), 1);
    EXPECT_EQ(m_queue.front(), 3);
}

TEST_F(QueueTest, DequeueThrowsOnEmptyQueue) { EXPECT_THROW(m_queue.dequeue(), std::out_of_range); }

TEST_F(QueueTest, FrontThrowsOnEmptyQueue) { EXPECT_THROW(m_queue.front(), std::out_of_range); }

TEST_F(QueueTest, BackThrowsOnEmptyQueue) { EXPECT_THROW(m_queue.back(), std::out_of_range); }

TEST_F(QueueTest, AtReturnsElementAtValidIndex) {
    m_queue.enqueue(1);
    m_queue.enqueue(2);
    m_queue.enqueue(3);

    EXPECT_EQ(m_queue.at(0), 1);
    EXPECT_EQ(m_queue.at(1), 2);
    EXPECT_EQ(m_queue.at(2), 3);
}

TEST_F(QueueTest, AtReturnsNulloptForInvalidIndex) {
    m_queue.enqueue(1);
    m_queue.enqueue(2);
    m_queue.enqueue(3);

    EXPECT_EQ(m_queue.at(3), std::nullopt);
}

TEST_F(QueueTest, ClearEmptiesQueue) {
    m_queue.enqueue(1);
    m_queue.enqueue(2);
    m_queue.enqueue(3);

    m_queue.clear();
    EXPECT_TRUE(m_queue.empty());
}

TEST_F(QueueTest, EmptyReturnsTrueOnEmptyQueue) { EXPECT_TRUE(m_queue.empty()); }

TEST_F(QueueTest, SizeReturnsZeroOnEmptyQueue) { EXPECT_EQ(m_queue.size(), 0); }

TEST_F(QueueTest, IteratorsWork) {
    m_queue.enqueue(1);
    m_queue.enqueue(2);
    m_queue.enqueue(3);

    std::size_t index{1};
    for (auto it = m_queue.begin(); it != m_queue.end(); ++it) {
        EXPECT_EQ(*it, index++);
    }

    index = 3;
    for (auto rit = m_queue.rbegin(); rit != m_queue.rend(); ++rit) {
        EXPECT_EQ(*rit, index--);
    }
}
