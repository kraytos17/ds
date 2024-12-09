#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstring>
#include <format>
#include <memory>
#include <ostream>
#include <span>
#include <string_view>
#include <variant>

class MyString {
public:
    constexpr MyString() noexcept : m_length{0}, m_storage{std::array<char, SSO_LEN>{}} {}

    constexpr explicit MyString(std::span<const char> span) : m_length{span.size()} {
        if (m_length <= SSO_LEN) {
            m_storage = std::array<char, SSO_LEN>{};
            std::copy(span.begin(), span.end(), std::get<std::array<char, SSO_LEN>>(m_storage).begin());
        } else {
            m_storage = std::make_unique<char[]>(m_length);
            std::copy(span.begin(), span.end(), std::get<std::unique_ptr<char[]>>(m_storage).get());
        }
    }

    MyString(const MyString& other) : m_length{other.m_length} {
        if (m_length <= SSO_LEN) {
            m_storage = std::get<std::array<char, SSO_LEN>>(other.m_storage);
        } else {
            m_storage = std::make_unique<char[]>(m_length);
            std::copy(other.begin(), other.end(), std::get<std::unique_ptr<char[]>>(m_storage).get());
        }
    }

    MyString(MyString&& other) noexcept : m_length{other.m_length}, m_storage{std::move(other.m_storage)} {
        other.m_length = 0;
    }

    MyString& operator=(const MyString& other) {
        if (this != &other) {
            m_length = other.m_length;

            if (m_length <= SSO_LEN) {
                m_storage = std::get<std::array<char, SSO_LEN>>(other.m_storage);
            } else {
                m_storage = std::make_unique<char[]>(m_length);
                std::copy(other.begin(), other.end(), std::get<std::unique_ptr<char[]>>(m_storage).get());
            }
        }
        return *this;
    }

    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            m_length = other.m_length;
            m_storage = std::move(other.m_storage);
            other.m_length = 0;
        }
        return *this;
    }

    ~MyString() = default;

    constexpr auto operator<=>(const MyString& other) const noexcept {
        if (m_length != other.m_length) {
            return m_length <=> other.m_length;
        }
        return std::lexicographical_compare_three_way(begin(), end(), other.begin(), other.end());
    }

    MyString operator+(const MyString& other) const {
        std::size_t newLen = m_length + other.m_length;
        MyString res;
        res.m_length = newLen;

        if (newLen <= SSO_LEN) {
            res.m_storage = std::array<char, SSO_LEN>{};
            auto res_data = std::get<std::array<char, SSO_LEN>>(res.m_storage).begin();
            std::copy(begin(), end(), res_data);
            std::copy(other.begin(), other.end(), res_data + m_length);
        } else {
            res.m_storage = std::make_unique<char[]>(newLen);
            auto res_data = std::get<std::unique_ptr<char[]>>(res.m_storage).get();
            std::copy(begin(), end(), res_data);
            std::copy(other.begin(), other.end(), res_data + m_length);
        }

        return res;
    }

    [[nodiscard]] constexpr std::size_t size() const noexcept { return m_length; }

    [[nodiscard]] constexpr const char* begin() const noexcept {
        return m_length <= SSO_LEN ? std::get<std::array<char, SSO_LEN>>(m_storage).data()
                                   : std::get<std::unique_ptr<char[]>>(m_storage).get();
    }

    [[nodiscard]] constexpr const char* end() const noexcept { return begin() + m_length; }

    [[nodiscard]] constexpr char* begin() noexcept {
        return m_length <= SSO_LEN ? std::get<std::array<char, SSO_LEN>>(m_storage).data()
                                   : std::get<std::unique_ptr<char[]>>(m_storage).get();
    }

    [[nodiscard]] constexpr char* end() noexcept { return begin() + m_length; }

    friend std::ostream& operator<<(std::ostream& stream, const MyString& string) noexcept {
        stream.write(string.begin(), string.size());
        return stream;
    }

private:
    static constexpr std::size_t SSO_LEN = 32;

    std::size_t m_length{};
    std::variant<std::array<char, SSO_LEN>, std::unique_ptr<char[]>> m_storage;
};

template<>
struct std::formatter<MyString> : std::formatter<std::string_view> {
    constexpr auto format(const MyString& string, auto& ctx) const {
        return std::formatter<std::string_view>::format(std::string_view{string.begin(), string.size()}, ctx);
    }
};
