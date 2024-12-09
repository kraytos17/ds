#include <gtest/gtest.h>
#include "../src/vector/my_vec.hpp"

TEST(MyVecTest, DefaultConstructor) {
    MyVec<int> v;

    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_TRUE(v.empty());
}

TEST(MyVecTest, ConstructorWithSizeAndValue) {
    MyVec<int> v(5, 42);

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v.capacity(), 5);
    for (std::size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 42);
    }
}

TEST(MyVecTest, CopyConstructor) {
    MyVec<int> original(3, 10);
    MyVec<int> copy = original;

    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy.capacity(), 3);
    for (std::size_t i = 0; i < copy.size(); ++i) {
        EXPECT_EQ(copy[i], 10);
    }
}

TEST(MyVecTest, MoveConstructor) {
    MyVec<int> original(3, 7);
    MyVec<int> moved = std::move(original);

    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.capacity(), 3);
    for (std::size_t i = 0; i < moved.size(); ++i) {
        EXPECT_EQ(moved[i], 7);
    }
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(original.capacity(), 0);
}

TEST(MyVecTest, AssignmentOperator) {
    MyVec<int> a(3, 1);
    MyVec<int> b;
    b = a;

    EXPECT_EQ(b.size(), 3);
    EXPECT_EQ(b.capacity(), 3);
    for (std::size_t i = 0; i < b.size(); ++i) {
        EXPECT_EQ(b[i], 1);
    }
}

TEST(MyVecTest, PushBack) {
    MyVec<int> v;

    v.pushBack(10);
    v.pushBack(20);

    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
}

TEST(MyVecTest, PopBack) {
    MyVec<int> v(3, 5);

    EXPECT_EQ(v.size(), 3);

    v.popBack();
    EXPECT_EQ(v.size(), 2);

    v.popBack();
    EXPECT_EQ(v.size(), 1);

    v.popBack();
    EXPECT_EQ(v.size(), 0);
}

TEST(MyVecTest, InsertSingleElement) {
    MyVec<int> v(3, 1);
    v.insert(1, 42);

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[1], 42);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[2], 1);
}

TEST(MyVecTest, EraseSingleElement) {
    MyVec<int> v{1, 2, 3, 4};

    v.erase(1);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 3);
    EXPECT_EQ(v[2], 4);
}

TEST(MyVecTest, ResizeLarger) {
    MyVec<int> v(3, 1);

    v.resize(5, 42);

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[3], 42);
    EXPECT_EQ(v[4], 42);
}

TEST(MyVecTest, ResizeSmaller) {
    MyVec<int> v(5, 1);

    v.resize(2);

    EXPECT_EQ(v.size(), 2);
}

TEST(MyVecTest, ShrinkToFit) {
    MyVec<int> v(5, 7);
    v.reserve(20);

    EXPECT_GE(v.capacity(), 20);

    v.shrinkToFit();

    EXPECT_EQ(v.capacity(), v.size());
    EXPECT_EQ(v.size(), 5);
    for (std::size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 7);
    }
}

TEST(MyVecTest, Assign) {
    MyVec<int> v;

    v.assign(4, 3);

    EXPECT_EQ(v.size(), 4);
    for (std::size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 3);
    }
}

TEST(MyVecTest, Iterators) {
    MyVec<int> v{1, 2, 3, 4};

    std::vector<int> copy(v.begin(), v.end());

    EXPECT_EQ(copy.size(), 4);
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[1], 2);
    EXPECT_EQ(copy[2], 3);
    EXPECT_EQ(copy[3], 4);
}

TEST(MyVecTest, ReverseIterators) {
    MyVec<int> v{1, 2, 3, 4};

    auto it = v.rbegin();
    EXPECT_EQ(*it, 4);
    ++it;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 1);
}
