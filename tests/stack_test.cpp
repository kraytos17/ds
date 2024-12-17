#include <gtest/gtest.h>
#include "../src/stack/my_stack.hpp"

class StackTest : public testing::Test {
protected:
    void SetUp() override {
        m_stackEmpty = Stack<int>();
        m_stackOne = Stack<int>();
        m_stackTwo = Stack<int>();
    }

    Stack<int> m_stackEmpty;
    Stack<int> m_stackOne;
    Stack<int> m_stackTwo;
};

TEST_F(StackTest, DefaultConstructor) {
    EXPECT_TRUE(m_stackEmpty.empty());
    EXPECT_EQ(m_stackEmpty.size(), 0);
}

TEST_F(StackTest, Push) {
    m_stackOne.push(10);
    m_stackOne.push(20);

    EXPECT_EQ(m_stackOne.top(), 20);
    EXPECT_EQ(m_stackOne.size(), 2);
    EXPECT_FALSE(m_stackOne.empty());
}

TEST_F(StackTest, Pop) {
    m_stackOne.push(10);
    m_stackOne.push(20);

    m_stackOne.pop();
    EXPECT_EQ(m_stackOne.top(), 10);
    EXPECT_EQ(m_stackOne.size(), 1);

    m_stackOne.pop();
    EXPECT_TRUE(m_stackOne.empty());
    EXPECT_EQ(m_stackOne.size(), 0);

    EXPECT_THROW(m_stackEmpty.pop(), std::out_of_range);
}

TEST_F(StackTest, Top) {
    m_stackTwo.push(100);
    EXPECT_EQ(m_stackTwo.top(), 100);

    const auto& const_stack = m_stackTwo;
    EXPECT_EQ(const_stack.top(), 100);
}

TEST_F(StackTest, EmptyStackTopException) { EXPECT_THROW(m_stackEmpty.top(), std::out_of_range); }

TEST_F(StackTest, SizeAndEmpty) {
    EXPECT_TRUE(m_stackEmpty.empty());
    EXPECT_EQ(m_stackEmpty.size(), 0);

    m_stackOne.push(1);
    m_stackOne.push(2);

    EXPECT_EQ(m_stackOne.size(), 2);
    EXPECT_FALSE(m_stackOne.empty());

    m_stackOne.pop();
    EXPECT_EQ(m_stackOne.size(), 1);
    EXPECT_FALSE(m_stackOne.empty());

    m_stackOne.pop();
    EXPECT_TRUE(m_stackOne.empty());
    EXPECT_EQ(m_stackOne.size(), 0);
}

TEST_F(StackTest, SizeAfterPushAndPop) {
    Stack<int> stack;

    EXPECT_EQ(stack.size(), 0);

    stack.push(10);
    EXPECT_EQ(stack.size(), 1);

    stack.push(20);
    EXPECT_EQ(stack.size(), 2);

    stack.pop();
    EXPECT_EQ(stack.size(), 1);

    stack.pop();
    EXPECT_EQ(stack.size(), 0);

    EXPECT_TRUE(stack.empty());
}

TEST_F(StackTest, Clear) {
    m_stackOne.push(10);
    m_stackOne.push(20);

    EXPECT_FALSE(m_stackOne.empty());
    EXPECT_EQ(m_stackOne.size(), 2);

    m_stackOne.clear();
    EXPECT_TRUE(m_stackOne.empty());
    EXPECT_EQ(m_stackOne.size(), 0);
}

TEST_F(StackTest, ElementsIteration) {
    m_stackTwo.push(1);
    m_stackTwo.push(2);
    m_stackTwo.push(3);

    std::vector<int> expected_elements{3, 2, 1};
    std::vector<int> actual_elements;

    for (const auto& elem: m_stackTwo.elements()) {
        actual_elements.push_back(elem);
    }

    EXPECT_EQ(actual_elements, expected_elements);
}

TEST_F(StackTest, StringStack) {
    Stack<std::string> string_stack;

    string_stack.push("Hello");
    string_stack.push("World");

    EXPECT_EQ(string_stack.top(), "World");
    EXPECT_EQ(string_stack.size(), 2);

    string_stack.pop();
    EXPECT_EQ(string_stack.top(), "Hello");
    EXPECT_EQ(string_stack.size(), 1);
}
