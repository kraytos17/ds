#include <gtest/gtest.h>
#include "../src/queue/my_cqueue.hpp"

class MyCircularQueueTest : public ::testing::Test {
protected:
    CircularQueue<int, 3> queue;
};

TEST_F(MyCircularQueueTest, EnqueueAndCheckFrontBack) {
    queue.enqueue(10);
    EXPECT_EQ(queue.front(), 10);
    EXPECT_EQ(queue.back(), 10);

    queue.enqueue(20);
    EXPECT_EQ(queue.front(), 10);
    EXPECT_EQ(queue.back(), 20);

    queue.enqueue(30);
    EXPECT_EQ(queue.front(), 10);
    EXPECT_EQ(queue.back(), 30);
}

TEST_F(MyCircularQueueTest, EnqueueFullThrows) {
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);
    EXPECT_THROW(queue.enqueue(4), std::out_of_range);
}

TEST_F(MyCircularQueueTest, DequeueAndCheckFront) {
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);

    queue.dequeue();
    EXPECT_EQ(queue.front(), 20);
    queue.dequeue();
    EXPECT_EQ(queue.front(), 30);
}

TEST_F(MyCircularQueueTest, DequeueEmptyThrows) { EXPECT_THROW(queue.dequeue(), std::underflow_error); }

TEST_F(MyCircularQueueTest, FullAndEmptyConditions) {
    EXPECT_TRUE(queue.empty());
    EXPECT_FALSE(queue.full());

    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);

    EXPECT_TRUE(queue.full());
    EXPECT_FALSE(queue.empty());

    queue.dequeue();
    EXPECT_FALSE(queue.full());
    EXPECT_FALSE(queue.empty());
}

TEST_F(MyCircularQueueTest, SizeValidation) {
    EXPECT_EQ(queue.size(), 0);

    queue.enqueue(5);
    EXPECT_EQ(queue.size(), 1);

    queue.enqueue(10);
    EXPECT_EQ(queue.size(), 2);

    queue.dequeue();
    EXPECT_EQ(queue.size(), 1);
}
