#include <gtest/gtest.h>
#include <sstream>
#include "../src/linked_list/my_dll.hpp"

class MyDllTest : public ::testing::Test {
protected:
    MyDll<int> list;

    std::string capturePrintOutput() {
        std::ostringstream output;
        std::streambuf* originalBuf = std::cout.rdbuf(output.rdbuf());

        list.print();
        std::cout.rdbuf(originalBuf);
        return output.str();
    }
};

TEST_F(MyDllTest, InsertFrontTest) {
    list.insertFront(10);
    list.insertFront(20);
    list.insertFront(30);

    std::string expected = "30 20 10 ";
    EXPECT_EQ(expected, capturePrintOutput());
}

TEST_F(MyDllTest, InsertBackTest) {
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);

    std::string expected = "10 20 30 ";
    EXPECT_EQ(expected, capturePrintOutput());
}

TEST_F(MyDllTest, RemoveFrontTest) {
    list.insertFront(10);
    list.insertFront(20);
    list.removeFront();

    std::string expected = "10 ";
    EXPECT_EQ(expected, capturePrintOutput());
}

TEST_F(MyDllTest, RemoveBackTest) {
    list.insertBack(10);
    list.insertBack(20);
    list.removeBack();

    std::string expected = "10 ";
    EXPECT_EQ(expected, capturePrintOutput());
}

TEST_F(MyDllTest, RemoveAtTest) {
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);
    list.removeAt(1);

    std::string expected = "10 30 ";
    EXPECT_EQ(expected, capturePrintOutput());
}

TEST_F(MyDllTest, FindTest) {
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);

    EXPECT_TRUE(list.find(20));
    EXPECT_FALSE(list.find(40));
}

TEST_F(MyDllTest, GetAtTest) {
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);

    EXPECT_EQ(list.getAt(0), 10);
    EXPECT_EQ(list.getAt(1), 20);
    EXPECT_EQ(list.getAt(2), 30);
}

TEST_F(MyDllTest, ClearTest) {
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);
    list.clear();

    EXPECT_TRUE(list.isEmpty());
}

TEST_F(MyDllTest, ReverseTest) {
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);
    list.reverse();

    std::string expected = "30 20 10 ";
    EXPECT_EQ(expected, capturePrintOutput());
}
