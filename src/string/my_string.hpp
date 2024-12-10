#pragma once

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <ostream>
#include <string_view>

class MyString {
public:
    static constexpr std::size_t SSO_LEN{31};

    constexpr MyString() noexcept : m_length{0} { m_sso[0] = '\0'; }

    MyString(const char* str) : m_length{std::strlen(str)} {
        if (m_length <= SSO_LEN) {
            std::memcpy(m_sso, str, m_length + 1);
        } else {
            m_data = new char[m_length + 1];
            std::memcpy(m_data, str, m_length + 1);
        }
    }

    MyString(std::string_view view) : m_length{view.size()} {
        if (m_length <= SSO_LEN) {
            std::memcpy(m_sso, view.data(), m_length);
            m_sso[m_length] = '\0';
        } else {
            m_data = new char[m_length + 1];
            std::memcpy(m_data, view.data(), m_length);
            m_data[m_length] = '\0';
        }
    }

    MyString(const MyString& other) : m_length{other.m_length} {
        if (other.is_sso()) {
            std::memcpy(m_sso, other.m_sso, SSO_LEN + 1);
        } else {
            m_data = new char[m_length + 1];
            std::memcpy(m_data, other.m_data, m_length + 1);
        }
    }

    MyString(MyString&& other) noexcept : m_length{other.m_length} {
        if (other.is_sso()) {
            std::memcpy(m_sso, other.m_sso, SSO_LEN + 1);
        } else {
            m_data = other.m_data;
            other.m_data = nullptr;
        }
        other.m_length = 0;
    }


    MyString& operator=(const MyString& other) {
        if (this != &other) {
            clear();
            m_length = other.m_length;

            if (other.is_sso()) {
                std::memcpy(m_sso, other.m_sso, SSO_LEN + 1);
            } else {
                m_data = new char[m_length + 1];
                std::memcpy(m_data, other.m_data, m_length + 1);
            }
        }
        return *this;
    }

    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            clear();

            m_length = other.m_length;
            if (other.is_sso()) {
                std::memcpy(m_sso, other.m_sso, SSO_LEN + 1);
            } else {
                m_data = other.m_data;
                other.m_data = nullptr;
            }

            other.m_length = 0;
        }
        return *this;
    }


    ~MyString() { clear(); }

    MyString operator+(const MyString& other) const {
        std::size_t new_length = m_length + other.m_length;
        MyString result;
        if (new_length <= SSO_LEN) {
            std::memcpy(result.m_sso, data(), m_length);
            std::memcpy(result.m_sso + m_length, other.data(), other.m_length);
            result.m_sso[new_length] = '\0';
        } else {
            result.m_data = new char[new_length + 1];
            std::memcpy(result.m_data, data(), m_length);
            std::memcpy(result.m_data + m_length, other.data(), other.m_length);
            result.m_data[new_length] = '\0';
        }
        result.m_length = new_length;
        return result;
    }

    constexpr auto operator<=>(const MyString& other) const noexcept {
        return std::lexicographical_compare_three_way(data(), data() + m_length, other.data(),
                                                      other.data() + other.m_length);
    }

    constexpr bool operator==(const MyString& other) const noexcept {
        return m_length == other.m_length && std::memcmp(data(), other.data(), m_length) == 0;
    }

    [[nodiscard]] constexpr std::size_t size() const noexcept { return m_length; }
    [[nodiscard]] const char* c_str() const noexcept { return is_sso() ? m_sso : m_data; }
    [[nodiscard]] const char* data() const noexcept { return c_str(); }

    friend std::ostream& operator<<(std::ostream& os, const MyString& str) {
        os << str.c_str();
        return os;
    }

private:
    union {
        char m_sso[SSO_LEN + 1]{};
        char* m_data;
    };

    std::size_t m_length{};

    [[nodiscard]] constexpr bool is_sso() const noexcept { return m_length <= SSO_LEN; }

    void clear() noexcept {
        if (!is_sso() && m_data) {
            delete[] m_data;
            m_data = nullptr;
        }
        m_length = 0;
    }
};
