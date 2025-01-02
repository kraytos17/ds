#pragma once

#include <bit>
#include <cmath>
#include <cstddef>
#include <deque>
#include <functional>
#include <optional>
#include <vector>

template<typename Key, typename Val, typename Hash = std::hash<Key>>
class HashMap {
private:
    struct Kvp {
        Key key;
        Val value;

        constexpr Kvp(Key k, Val v) : key(std::move(k)), value(std::move(v)) {}
        constexpr auto operator<=>(const Kvp&) const noexcept = default;
    };

public:
    constexpr HashMap(std::size_t cap = 16) : m_table(cap) {}

    constexpr auto operator<=>(const HashMap&) const noexcept = default;

    Val& operator[](const Key& k) {
        auto& bucket = m_table[m_hash(k) % m_table.size()];
        if (auto it = std::ranges::find(bucket, k, &Kvp::key); it != bucket.end()) {
            return it->value;
        }
    }

    void reserve(size_t n) {
        size_t needed_buckets = std::ceil(n / m_loadFactor);
        if (needed_buckets > m_table.size()) {
            rehash(needed_buckets);
        }
    }

    void shrink_to_fit() {
        size_t optimal_buckets = std::ceil(m_size / m_loadFactor);
        optimal_buckets = std::max(optimal_buckets, size_t{16});
        if (optimal_buckets < m_table.size()) {
            rehash(optimal_buckets);
        }
    }

    constexpr float loadFactor() const { return static_cast<float>(m_size) / m_table.size(); }

    void rehash(size_t n) {
        n = std::max(n, size_t{16});
        n = std::bit_ceil(n);
        std::vector<std::deque<Kvp>> new_table(n);
        for (auto& bucket: m_table) {
            for (auto& kvp: bucket) {
                new_table[m_hash(kvp.key) % n].push_back(std::move(kvp));
            }
        }

        m_table = std::move(new_table);
    }

    std::optional<const Val&> get(this const auto& self, const Key& k) {
        auto& bucket = self.m_table[self.m_hash(k) % self.m_table.size()];
        if (auto it = std::ranges::find(bucket, k, &Kvp::key); it != bucket.end()) {
            return it->value;
        }
        return std::nullopt;
    }

    void insert(const Key& k, Val v) {
        if (m_size >= m_table.size() * m_loadFactor) {
            resize();
        }

        auto& bucket = m_table[m_hash(k) % m_table.size()];
        if (auto it = std::ranges::find(bucket, k, &Kvp::key); it != bucket.end()) {
            it->value = std::move(v);
            return;
        }
        bucket.emplace_back(k, std::move(v));
        ++m_size;
    }

    bool contains(this const auto& self, const Key& k) {
        auto& bucket = self.m_table[self.m_hash(k) % self.m_table.size()];
        return std::ranges::find(bucket, k, &Kvp::key) != bucket.end();
    }

    bool erase(const Key& k) {
        auto& bucket = m_table[m_hash(k) % m_table.size()];
        if (auto it = std::ranges::find(bucket, k, &Kvp::key); it != bucket.end()) {
            bucket.erase(it);
            --m_size;
            return true;
        }
        return false;
    }

    constexpr std::size_t size() const { return m_size; }
    constexpr bool empty() const { return m_size == 0; }
    constexpr std::size_t bucket_count() const { return m_table.size(); }

private:
    std::vector<std::deque<Kvp>> m_table;
    std::size_t m_size{0};
    static constexpr float m_loadFactor{0.75f};
    Hash m_hash{};

    void resize() {
        std::vector<std::deque<Kvp>> new_table(m_table.size() * 2);
        for (auto& bucket: m_table) {
            for (auto& kvp: bucket) {
                new_table[m_hash(kvp.key) % new_table.size()].push_back(std::move(kvp));
            }
        }
        m_table = std::move(new_table);
    }
};
