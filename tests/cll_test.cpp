#include <gtest/gtest.h>
#include <sstream>
#include "../src/linked_list/my_cll.hpp"

class MyCllTest : public ::testing::Test {
protected:
    CLL<int> list;

    void SetUp() override {
        list.pushBack(1);
        list.pushBack(2);
        list.pushBack(3);
    }
};

TEST_F(MyCllTest, PushBack_IncreasesSizeAndAppends) {
    EXPECT_EQ(list.size(), 3);
    EXPECT_FALSE(list.empty());

    list.pushBack(4);
    EXPECT_EQ(list.size(), 4);
    EXPECT_EQ(list.back(), 4);
}

TEST_F(MyCllTest, PushFront_InsertsAtFront) {
    EXPECT_EQ(list.front(), 1);

    list.pushFront(0);
    EXPECT_EQ(list.size(), 4);
    EXPECT_EQ(list.front(), 0);
    EXPECT_EQ(list.back(), 3);
}

TEST_F(MyCllTest, PopFront_RemovesFrontElement) {
    EXPECT_EQ(list.front(), 1);

    list.popFront();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 2);

    list.popFront();
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), 3);
}

TEST_F(MyCllTest, PopBack_RemovesBackElement) {
    EXPECT_EQ(list.back(), 3);

    list.popBack();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.back(), 2);

    list.popBack();
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.back(), 1);
}

TEST_F(MyCllTest, Remove_RemovesSpecifiedElement) {
    list.remove(2);
    EXPECT_EQ(list.size(), 2);

    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);

    list.remove(1);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), 3);
    EXPECT_EQ(list.back(), 3);
}

TEST_F(MyCllTest, Clear_EmptiesTheList) {
    list.clear();
    EXPECT_EQ(list.size(), 0);
    EXPECT_TRUE(list.empty());

    EXPECT_THROW(list.front(), std::out_of_range);
    EXPECT_THROW(list.back(), std::out_of_range);
}

TEST_F(MyCllTest, Iterator_TraversesTheList) {
    std::vector<int> values;
    for (const auto& val: list) {
        values.push_back(val);
    }

    EXPECT_EQ(values.size(), 3);
    EXPECT_EQ(values[0], 1);
    EXPECT_EQ(values[1], 2);
    EXPECT_EQ(values[2], 3);
}

TEST_F(MyCllTest, Print_PrintsListContents) {
    std::ostringstream oss;
    list.print(oss);

    std::string expectedOutput = "1 -> 2 -> 3 -> HEAD (circular)\n";
    EXPECT_EQ(oss.str(), expectedOutput);
}

TEST(MyCllEdgeCaseTest, EmptyList_ThrowsOnAccess) {
    CLL<int> emptyList;

    EXPECT_TRUE(emptyList.empty());
    EXPECT_EQ(emptyList.size(), 0);

    EXPECT_THROW(emptyList.front(), std::out_of_range);
    EXPECT_THROW(emptyList.back(), std::out_of_range);
    EXPECT_THROW(emptyList.popFront(), std::out_of_range);
    EXPECT_THROW(emptyList.popBack(), std::out_of_range);
}
