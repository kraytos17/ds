#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "../src/string/my_string.hpp"

TEST(MyStringTest, DefaultConstructor) {
    MyString str;
    EXPECT_EQ(str.size(), 0);
    EXPECT_EQ(std::string_view(str.data(), str.size()), "");
}

TEST(MyStringTest, ConstructorWithEmptyString) {
    MyString str("");
    EXPECT_EQ(str.size(), 0);
    EXPECT_EQ(std::string_view(str.data(), str.size()), "");
}

TEST(MyStringTest, ConstructorWithSingleCharacter) {
    MyString str("A");
    EXPECT_EQ(str.size(), 1);
    EXPECT_EQ(std::string_view(str.data(), str.size()), "A");
}

TEST(MyStringTest, ConstructorWithSmallString) {
    const char* smallStr = "Hello";
    MyString str(smallStr);
    EXPECT_EQ(str.size(), 5);
    EXPECT_EQ(std::string_view(str.data(), str.size()), "Hello");
}

TEST(MyStringTest, ConstructorWithSSOString) {
    const char* ssoStr = "1234567890123456789012345678901"; // Exactly SSO_LEN (31 chars)
    MyString str(ssoStr);
    EXPECT_EQ(str.size(), 31);
    EXPECT_EQ(std::string_view(str.data(), str.size()), ssoStr);
}

TEST(MyStringTest, ConstructorWithLargeString) {
    std::string largeStr(50, 'A'); // Exceeds SSO_LEN
    MyString str(largeStr);
    EXPECT_EQ(str.size(), 50);
    EXPECT_EQ(std::string_view(str.data(), str.size()), largeStr);
}

TEST(MyStringTest, CopyConstructor) {
    MyString original("Hello");
    MyString copy = original;
    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(std::string_view(copy.data(), copy.size()), "Hello");
}

TEST(MyStringTest, MoveConstructor) {
    MyString original("Hello");
    MyString moved = std::move(original);

    EXPECT_EQ(moved.size(), 5);
    EXPECT_EQ(std::string_view(moved.data(), moved.size()), "Hello");
    EXPECT_EQ(original.size(), 0);
}

TEST(MyStringTest, CopyAssignment) {
    MyString original("Hello");
    MyString copy;
    copy = original;

    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(std::string_view(copy.data(), copy.size()), "Hello");
}

TEST(MyStringTest, MoveAssignment) {
    MyString original("Hello");
    MyString moved;
    moved = std::move(original);

    EXPECT_EQ(moved.size(), 5);
    EXPECT_EQ(std::string_view(moved.data(), moved.size()), "Hello");
    EXPECT_EQ(original.size(), 0);
}

TEST(MyStringTest, Concatenation) {
    MyString str1("Hello, ");
    MyString str2("World!");

    MyString result = str1 + str2;

    EXPECT_EQ(result.size(), 13);
    EXPECT_EQ(std::string_view(result.data(), result.size()), "Hello, World!");
}

TEST(MyStringTest, ConcatenationWithEmptyString) {
    MyString str1("Hello");
    MyString str2("");

    MyString result = str1 + str2;

    EXPECT_EQ(result.size(), 5);
    EXPECT_EQ(std::string_view(result.data(), result.size()), "Hello");
}

TEST(MyStringTest, ConcatenationEmptyStrings) {
    MyString str1("");
    MyString str2("");

    MyString result = str1 + str2;

    EXPECT_EQ(result.size(), 0);
    EXPECT_EQ(std::string_view(result.data(), result.size()), "");
}

TEST(MyStringTest, OstreamOperator) {
    MyString str("Hello, World!");
    std::ostringstream os;
    os << str;

    EXPECT_EQ(os.str(), "Hello, World!");
}

TEST(MyStringTest, ComparisonOperator) {
    MyString str1("Apple");
    MyString str2("Banana");
    MyString str3("Apple");

    EXPECT_LT(str1, str2);
    EXPECT_EQ(str1, str3);
    EXPECT_GT(str2, str1);
}

TEST(MyStringTest, ComparisonWithEmptyString) {
    MyString str1("Apple");
    MyString str2("");

    EXPECT_GT(str1, str2);
    EXPECT_NE(str1, str2);
    EXPECT_LT(str2, str1);
}

TEST(MyStringTest, MoveSemantics) {
    MyString original("Move this!");
    MyString moved = std::move(original);

    EXPECT_EQ(std::string_view(moved.data(), moved.size()), "Move this!");
    EXPECT_EQ(original.size(), 0);
}

TEST(MyStringTest, CopySemantics) {
    MyString original("Copy this!");
    MyString copy = original;

    EXPECT_EQ(copy.size(), 10);
    EXPECT_EQ(std::string_view(copy.data(), copy.size()), "Copy this!");
    EXPECT_EQ(original.size(), 10);
}

TEST(MyStringTest, LongString) {
    std::string longStr(1000, 'A');
    MyString str(longStr);

    EXPECT_EQ(str.size(), 1000);
    EXPECT_EQ(std::string_view(str.data(), str.size()), longStr);
}

TEST(MyStringTest, ExactSSOEdgeCase) {
    const char* exactSSO = "1234567890123456789012345678901"; // Exactly 31 chars
    MyString str(exactSSO);
    EXPECT_EQ(str.size(), 31);
    EXPECT_EQ(std::string_view(str.data(), str.size()), exactSSO);
}

TEST(MyStringTest, LargeStringBeyondSSO) {
    const char* beyondSSO = "1234567890123456789012345678901234567890"; // 40 chars
    MyString str(beyondSSO);
    EXPECT_EQ(str.size(), 40);
    EXPECT_EQ(std::string_view(str.data(), str.size()), beyondSSO);
}
