#ifndef CPP_STRING_H
#define CPP_STRING_H

#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cstddef>

class String {
private:
    char* data_;
    std::size_t size_;

public:
    String() : data_(new char[1]{'\0'}), size_(0) {
        std::cout << "Default constructor called\n";
    }

    explicit String(const char* cstr) {
        if (cstr == nullptr) {
            size_ = 0;
            data_ = new char[1]{'\0'};
        }
        else {
            size_ = std::strlen(cstr);
            data_ = new char[size_ + 1];
            std::memcpy(data_, cstr, size_ + 1);
        }
        std::cout << "Parameterize constructor called\n";
    }

    String(const String& other) : size_(other.size_) {
        data_ = new char[size_ + 1];
        std::memcpy(data_, other.data_, size_ + 1);
        std::cout << "Copy constructor called\n";
    }

    String(String&& other) noexcept : data_(other.data_), size_(other.size_) {
        other.data_ = new char[1]{'\0'};
        other.size_ = 0;
        std::cout << "Move constructor called\n";
    }

    String& operator=(const String& other);

    String& operator=(String&& other) noexcept;

    [[nodiscard]] std::size_t size() const {
        return size_;
    }

    friend std::ostream& operator<<(std::ostream& out, const String& obj);

    friend std::istream& operator>>(std::istream& in, String& obj);

    char& operator[](std::size_t index);

    const char& operator[](std::size_t index) const;

    bool operator==(const String& other) const;

    bool operator>(const String& other) const;

    bool operator<(const String& other) const;

    ~String() {
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
        std::cout << "Destructor called" << std::endl;
    }
};

inline String& String::operator=(const String& other) {
    if (this == &other) {
        return *this;
    }
    char* new_data = new char[other.size_ + 1];
    std::memcpy(new_data, other.data_, other.size_ + 1);
    delete[] data_;
    data_ = new_data;
    size_ = other.size_;
    std::cout << "Assignment called\n";
    return *this;
}

inline String& String::operator=(String&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        other.data_ = new char[1]{'\0'};
        other.size_ = 0;
        std::cout << "Move assignment called\n";
    }
    return *this;
}

inline std::ostream& operator<<(std::ostream& out, const String& obj) {
    out << obj.data_;
    return out;
}

inline std::istream& operator>>(std::istream& in, String& obj) {
    char buffer[4096];
    if (in >> buffer) {
        const std::size_t new_size = std::strlen(buffer);
        char* new_data = new char[new_size + 1];
        std::memcpy(new_data, buffer, new_size + 1);
        delete[] obj.data_;
        obj.data_ = new_data;
        obj.size_ = new_size;
    }
    return in;
}

inline char& String::operator[](const std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Index out of bounds");
    }
    return data_[index];
}

inline const char& String::operator[](const std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Index out of bounds");
    }
    return data_[index];
}

inline bool String::operator==(const String& other) const {
    if (size_ != other.size_) {
        return false;
    }
    return std::strcmp(data_, other.data_) == 0;
}

inline bool String::operator>(const String& other) const {
    const std::size_t min_len = std::min(size_, other.size_);
    for (std::size_t i = 0; i < min_len; ++i) {
        if (data_[i] > other.data_[i]) {
            return true;
        }
        if (data_[i] < other.data_[i]) {
            return false;
        }
    }
    return (size_ > other.size_);
}

inline bool String::operator<(const String& other) const {
    const std::size_t min_len = std::min(size_, other.size_);
    for (std::size_t i = 0; i < min_len; ++i) {
        if (data_[i] < other.data_[i]) {
            return true;
        }
        if (data_[i] > other.data_[i]) {
            return false;
        }
    }
    return (size_ < other.size_);
}

#endif // CPP_STRING_H
