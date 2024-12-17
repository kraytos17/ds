#include <gtest/gtest.h>
#include <stdexcept>
#include "../src/linked_list/my_sll.hpp"

class MySllTest : public ::testing::Test {
protected:
    SLL<int> list;
};

TEST_F(MySllTest, InsertFront) {
    list.insertFront(10);
    list.insertFront(20);
    list.insertFront(30);

    // Expected order: 30 -> 20 -> 10
    EXPECT_EQ(list.getAt(0), 30);
    EXPECT_EQ(list.getAt(1), 20);
    EXPECT_EQ(list.getAt(2), 10);
}

TEST_F(MySllTest, InsertBack) {
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);

    // Expected order: 10 -> 20 -> 30
    EXPECT_EQ(list.getAt(0), 10);
    EXPECT_EQ(list.getAt(1), 20);
    EXPECT_EQ(list.getAt(2), 30);
}

TEST_F(MySllTest, InsertAt) {
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);

    list.insertAt(1, 15);

    // Expected order: 10 -> 15 -> 20 -> 30
    EXPECT_EQ(list.getAt(0), 10);
    EXPECT_EQ(list.getAt(1), 15);
    EXPECT_EQ(list.getAt(2), 20);
    EXPECT_EQ(list.getAt(3), 30);
}

TEST_F(MySllTest, RemoveFront) {
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);

    list.removeFront();

    // Expected order: 20 -> 30
    EXPECT_EQ(list.getAt(0), 20);
    EXPECT_EQ(list.getAt(1), 30);
}

TEST_F(MySllTest, RemoveBack) {
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);

    list.removeBack();

    // Expected order: 10 -> 20
    EXPECT_EQ(list.getAt(0), 10);
    EXPECT_EQ(list.getAt(1), 20);
}

TEST_F(MySllTest, RemoveAt) {
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);

    list.removeAt(1);

    // Expected order: 10 -> 30
    EXPECT_EQ(list.getAt(0), 10);
    EXPECT_EQ(list.getAt(1), 30);
}

TEST_F(MySllTest, Find) {
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);

    EXPECT_TRUE(list.find(20));
    EXPECT_FALSE(list.find(40));
}

TEST_F(MySllTest, FindIf) {
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);

    auto result = list.findIf([](const int& val) { return val > 15; });
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 20);

    result = list.findIf([](const int& val) { return val > 40; });
    EXPECT_FALSE(result.has_value());
}

TEST_F(MySllTest, GetAt) {
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);

    EXPECT_EQ(list.getAt(0), 10);
    EXPECT_EQ(list.getAt(1), 20);
    EXPECT_EQ(list.getAt(2), 30);
}

TEST_F(MySllTest, Count) {
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);

    EXPECT_EQ(list.count(), 3);
    list.removeBack();
    EXPECT_EQ(list.count(), 2);
}

TEST_F(MySllTest, Clear) {
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);

    list.clear();
    EXPECT_TRUE(list.isEmpty());
}

TEST_F(MySllTest, GetAtOutOfRange) {
    list.insertBack(10);

    EXPECT_THROW(list.getAt(1), std::out_of_range);
}

TEST_F(MySllTest, RemoveFromEmpty) {
    EXPECT_THROW(list.removeFront(), std::underflow_error);
    EXPECT_THROW(list.removeBack(), std::underflow_error);
    EXPECT_THROW(list.removeAt(0), std::out_of_range);
    EXPECT_TRUE(list.isEmpty());
}

TEST_F(MySllTest, Reverse) {
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);

    list.reverse(); // Expected order after reverse: 30 -> 20 -> 10

    EXPECT_EQ(list.getAt(0), 30);
    EXPECT_EQ(list.getAt(1), 20);
    EXPECT_EQ(list.getAt(2), 10);
}
