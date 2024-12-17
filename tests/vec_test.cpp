#include <gtest/gtest.h>
#include "../src/vector/my_vec.hpp"

class MyVecTest : public testing::Test {
protected:
    Vector<int> defaultVec;
    Vector<int> sizeValueVec = Vector<int>(5, 42);
};

TEST_F(MyVecTest, DefaultConstructor) {
    EXPECT_EQ(defaultVec.size(), 0);
    EXPECT_EQ(defaultVec.capacity(), 0);
    EXPECT_TRUE(defaultVec.empty());
}

TEST_F(MyVecTest, ConstructorWithSizeAndValue) {
    EXPECT_EQ(sizeValueVec.size(), 5);
    EXPECT_EQ(sizeValueVec.capacity(), 5);
    for (std::size_t i = 0; i < sizeValueVec.size(); ++i) {
        EXPECT_EQ(sizeValueVec[i], 42);
    }
}

TEST_F(MyVecTest, CopyConstructor) {
    Vector<int> copy = sizeValueVec;

    EXPECT_EQ(copy.size(), sizeValueVec.size());
    EXPECT_EQ(copy.capacity(), sizeValueVec.capacity());
    for (std::size_t i = 0; i < copy.size(); ++i) {
        EXPECT_EQ(copy[i], 42);
    }
}

TEST_F(MyVecTest, MoveConstructor) {
    Vector<int> moved = std::move(sizeValueVec);

    EXPECT_EQ(moved.size(), 5);
    EXPECT_EQ(moved.capacity(), 5);
    for (std::size_t i = 0; i < moved.size(); ++i) {
        EXPECT_EQ(moved[i], 42);
    }
    EXPECT_EQ(sizeValueVec.size(), 0);
    EXPECT_EQ(sizeValueVec.capacity(), 0);
}

TEST_F(MyVecTest, AssignmentOperator) {
    Vector<int> assigned;
    assigned = sizeValueVec;

    EXPECT_EQ(assigned.size(), sizeValueVec.size());
    EXPECT_EQ(assigned.capacity(), sizeValueVec.capacity());
    for (std::size_t i = 0; i < assigned.size(); ++i) {
        EXPECT_EQ(assigned[i], 42);
    }
}

TEST_F(MyVecTest, PushBack) {
    defaultVec.pushBack(10);
    defaultVec.pushBack(20);

    EXPECT_EQ(defaultVec.size(), 2);
    EXPECT_EQ(defaultVec[0], 10);
    EXPECT_EQ(defaultVec[1], 20);
}

TEST_F(MyVecTest, PopBack) {
    Vector<int> vec(3, 5);

    vec.popBack();
    EXPECT_EQ(vec.size(), 2);

    vec.popBack();
    EXPECT_EQ(vec.size(), 1);

    vec.popBack();
    EXPECT_EQ(vec.size(), 0);
}

TEST_F(MyVecTest, InsertSingleElement) {
    Vector<int> vec(3, 1);
    vec.insert(1, 42);

    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec[1], 42);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[2], 1);
}

TEST_F(MyVecTest, EraseSingleElement) {
    Vector<int> vec{1, 2, 3, 4};

    vec.erase(1);

    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 3);
    EXPECT_EQ(vec[2], 4);
}

TEST_F(MyVecTest, ResizeLarger) {
    Vector<int> vec(3, 1);

    vec.resize(5, 42);

    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[3], 42);
    EXPECT_EQ(vec[4], 42);
}

TEST_F(MyVecTest, ResizeSmaller) {
    Vector<int> vec(5, 1);

    vec.resize(2);

    EXPECT_EQ(vec.size(), 2);
}

TEST_F(MyVecTest, ShrinkToFit) {
    Vector<int> vec(5, 7);
    vec.reserve(20);

    EXPECT_GE(vec.capacity(), 20);

    vec.shrinkToFit();

    EXPECT_EQ(vec.capacity(), vec.size());
    EXPECT_EQ(vec.size(), 5);
    for (std::size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], 7);
    }
}

TEST_F(MyVecTest, Assign) {
    Vector<int> vec;
    vec.assign(4, 3);

    EXPECT_EQ(vec.size(), 4);
    for (std::size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], 3);
    }
}

TEST_F(MyVecTest, Iterators) {
    Vector<int> vec{1, 2, 3, 4};

    std::vector<int> copy(vec.begin(), vec.end());

    EXPECT_EQ(copy.size(), 4);
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[1], 2);
    EXPECT_EQ(copy[2], 3);
    EXPECT_EQ(copy[3], 4);
}

TEST_F(MyVecTest, ReverseIterators) {
    Vector<int> vec{1, 2, 3, 4};

    auto it = vec.rbegin();
    EXPECT_EQ(*it, 4);
    ++it;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 1);
}
