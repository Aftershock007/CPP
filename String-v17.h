#ifndef CPP_STRING_V17_H
#define CPP_STRING_V17_H

#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cstddef>
#include <cctype>
#include <functional>
#include <string_view>

class String {
    // SSO: strings <= SSO_CAP bytes are stored inline, zero heap allocations.
    // data_ always points to the active buffer (sso_buf_ or heap).
    static constexpr std::size_t SSO_CAP = 15;

    char sso_buf_[SSO_CAP + 1];
    char* data_;
    std::size_t size_;
    std::size_t capacity_;

    bool is_heap() const noexcept { return data_ != sso_buf_; }

    // Grow to at least new_cap. Moves data to heap if needed.
    void grow(std::size_t new_cap) {
        if (new_cap <= capacity_) return;
        char* new_data = new char[new_cap + 1];
        std::memcpy(new_data, data_, size_ + 1);
        if (is_heap()) delete[] data_;
        data_ = new_data;
        capacity_ = new_cap;
    }

public:
    static constexpr std::size_t npos = static_cast<std::size_t>(-1);

    // --- Constructors ---

    String() : size_(0), capacity_(SSO_CAP) {
        data_ = sso_buf_;
        sso_buf_[0] = '\0';
    }

    explicit String(const char* cstr) {
        if (cstr == nullptr) {
            data_ = sso_buf_;
            size_ = 0;
            capacity_ = SSO_CAP;
            sso_buf_[0] = '\0';
        } else {
            size_ = std::strlen(cstr);
            if (size_ <= SSO_CAP) {
                data_ = sso_buf_;
                capacity_ = SSO_CAP;
            } else {
                capacity_ = size_;
                data_ = new char[capacity_ + 1];
            }
            std::memcpy(data_, cstr, size_ + 1);
        }
    }

    // Construct from std::string_view — zero-copy into SSO or heap.
    explicit String(std::string_view sv) : size_(sv.size()) {
        if (size_ <= SSO_CAP) {
            data_ = sso_buf_;
            capacity_ = SSO_CAP;
        } else {
            capacity_ = size_;
            data_ = new char[capacity_ + 1];
        }
        std::copy(sv.begin(), sv.end(), data_);
        data_[size_] = '\0';
    }

    // Strong exception guarantee.
    String(const String& other) : size_(other.size_) {
        if (size_ <= SSO_CAP) {
            data_ = sso_buf_;
            capacity_ = SSO_CAP;
        } else {
            capacity_ = size_;
            data_ = new char[capacity_ + 1];
        }
        std::memcpy(data_, other.data_, size_ + 1);
    }

    // Nothrow guarantee. No allocation — steals heap pointer or copies SSO bytes.
    String(String&& other) noexcept : size_(other.size_), capacity_(other.capacity_) {
        if (other.is_heap()) {
            data_ = other.data_;
        } else {
            data_ = sso_buf_;
            std::memcpy(sso_buf_, other.sso_buf_, size_ + 1);
        }
        other.data_ = other.sso_buf_;
        other.size_ = 0;
        other.capacity_ = SSO_CAP;
        other.sso_buf_[0] = '\0';
    }

    // --- Assignment (CAS) ---

    void swap(String& other) noexcept {
        const bool this_heap = is_heap();
        const bool other_heap = other.is_heap();

        if (this_heap && other_heap) {
            std::swap(data_, other.data_);
        } else if (!this_heap && !other_heap) {
            char temp[SSO_CAP + 1];
            std::memcpy(temp, sso_buf_, SSO_CAP + 1);
            std::memcpy(sso_buf_, other.sso_buf_, SSO_CAP + 1);
            std::memcpy(other.sso_buf_, temp, SSO_CAP + 1);
            data_ = sso_buf_;
            other.data_ = other.sso_buf_;
        } else {
            String& h = this_heap ? *this : other;
            String& s = this_heap ? other : *this;
            char* heap_ptr = h.data_;
            std::size_t heap_cap = h.capacity_;
            std::memcpy(h.sso_buf_, s.sso_buf_, SSO_CAP + 1);
            h.data_ = h.sso_buf_;
            h.capacity_ = SSO_CAP;
            s.data_ = heap_ptr;
            s.capacity_ = heap_cap;
        }
        std::swap(size_, other.size_);
    }

    // Strong exception guarantee via CAS.
    String& operator=(String other) noexcept {
        other.swap(*this);
        return *this;
    }

    // --- Getters ---

    [[nodiscard]] std::size_t size() const noexcept { return size_; }
    [[nodiscard]] std::size_t capacity() const noexcept { return capacity_; }
    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }
    [[nodiscard]] const char* c_str() const noexcept { return data_; }

    // --- Implicit conversion to std::string_view (zero-copy) ---

    operator std::string_view() const noexcept { return {data_, size_}; }

    // --- Iterators ---

    char* begin() noexcept { return data_; }
    char* end() noexcept { return data_ + size_; }
    const char* begin() const noexcept { return data_; }
    const char* end() const noexcept { return data_ + size_; }

    // --- Element access ---

    char& operator[](std::size_t index);
    const char& operator[](std::size_t index) const;

    // --- Modifiers ---

    void clear() {
        size_ = 0;
        data_[0] = '\0';
    }

    void reserve(std::size_t new_cap);

    String& append(const String& other);
    String& append(const char* cstr);
    String& operator+=(const String& other);
    String& operator+=(const char* cstr);

    // --- Operations ---

    String substr(std::size_t pos, std::size_t len = npos) const;
    [[nodiscard]] std::size_t find(char ch, std::size_t pos = 0) const;
    [[nodiscard]] std::size_t find(const String& str, std::size_t pos = 0) const;

    // --- Comparison (strcmp-based) ---

    bool operator==(const String& other) const;
    bool operator>(const String& other) const;
    bool operator<(const String& other) const;

    // --- Stream I/O ---

    friend std::ostream& operator<<(std::ostream& out, const String& obj);
    friend std::istream& operator>>(std::istream& in, String& obj);

    // --- Destructor ---

    ~String() {
        if (is_heap()) delete[] data_;
    }
};

// --- Non-member operator+ ---

inline String operator+(const String& lhs, const String& rhs) {
    String result(lhs);
    result += rhs;
    return result;
}

// --- Inline definitions ---

inline void String::reserve(const std::size_t new_cap) {
    if (new_cap <= capacity_) return;
    grow(new_cap);
}

// Strong exception guarantee.
inline String& String::append(const String& other) {
    if (other.size_ == 0) return *this;
    const std::size_t new_size = size_ + other.size_;
    if (new_size > capacity_) {
        grow(std::max(new_size, capacity_ * 2));
    }
    std::memcpy(data_ + size_, other.data_, other.size_);
    size_ = new_size;
    data_[size_] = '\0';
    return *this;
}

inline String& String::append(const char* cstr) {
    if (cstr == nullptr) return *this;
    const std::size_t len = std::strlen(cstr);
    if (len == 0) return *this;
    const std::size_t new_size = size_ + len;
    if (new_size > capacity_) {
        grow(std::max(new_size, capacity_ * 2));
    }
    std::memcpy(data_ + size_, cstr, len);
    size_ = new_size;
    data_[size_] = '\0';
    return *this;
}

inline String& String::operator+=(const String& other) {
    return append(other);
}

inline String& String::operator+=(const char* cstr) {
    return append(cstr);
}

inline String String::substr(const std::size_t pos, std::size_t len) const {
    if (pos > size_) {
        throw std::out_of_range("substr position out of bounds");
    }
    if (len == npos || pos + len > size_) {
        len = size_ - pos;
    }
    String result;
    if (len > 0) {
        if (len > result.capacity_) {
            result.grow(len);
        }
        std::memcpy(result.data_, data_ + pos, len);
        result.size_ = len;
        result.data_[len] = '\0';
    }
    return result;
}

inline std::size_t String::find(const char ch, const std::size_t pos) const {
    for (std::size_t i = pos; i < size_; ++i) {
        if (data_[i] == ch) return i;
    }
    return npos;
}

inline std::size_t String::find(const String& str, const std::size_t pos) const {
    if (str.size_ == 0) return pos <= size_ ? pos : npos;
    if (str.size_ > size_) return npos;
    for (std::size_t i = pos; i <= size_ - str.size_; ++i) {
        if (std::memcmp(data_ + i, str.data_, str.size_) == 0) {
            return i;
        }
    }
    return npos;
}

inline char& String::operator[](const std::size_t index) {
    if (index >= size_) throw std::out_of_range("Index out of bounds");
    return data_[index];
}

inline const char& String::operator[](const std::size_t index) const {
    if (index >= size_) throw std::out_of_range("Index out of bounds");
    return data_[index];
}

inline bool String::operator==(const String& other) const {
    if (size_ != other.size_) return false;
    return std::memcmp(data_, other.data_, size_) == 0;
}

inline bool String::operator>(const String& other) const {
    return std::strcmp(data_, other.data_) > 0;
}

inline bool String::operator<(const String& other) const {
    return std::strcmp(data_, other.data_) < 0;
}

inline std::ostream& operator<<(std::ostream& out, const String& obj) {
    out << obj.data_;
    return out;
}

// Dynamic growth — no fixed buffer limit, reads until whitespace.
inline std::istream& operator>>(std::istream& in, String& obj) {
    obj.clear();
    char ch;
    // Skip leading whitespace
    while (in.get(ch) && std::isspace(static_cast<unsigned char>(ch))) {}
    if (!in) return in;
    // Read non-whitespace characters
    do {
        if (obj.size_ == obj.capacity_) {
            obj.grow(obj.capacity_ * 2);
        }
        obj.data_[obj.size_++] = ch;
    } while (in.get(ch) && !std::isspace(static_cast<unsigned char>(ch)));
    obj.data_[obj.size_] = '\0';
    if (in) in.putback(ch);
    return in;
}

// --- std::hash specialization (FNV-1a) ---

template <>
struct std::hash<String> {
    std::size_t operator()(const String& s) const noexcept {
        std::size_t hash = 14695981039346656037ULL;
        const char* data = s.c_str();
        for (std::size_t i = 0; i < s.size(); ++i) {
            hash ^= static_cast<unsigned char>(data[i]);
            hash *= 1099511628211ULL;
        }
        return hash;
    }
};

#endif // CPP_STRING_V17_H
