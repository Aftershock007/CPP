#ifndef CPP_VECTOR_H
#define CPP_VECTOR_H

#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <algorithm>

template <typename T>
class Vector {
private:
    T* data_;
    std::size_t size_;

public:
    Vector() : data_(nullptr), size_(0) {}

    explicit Vector(const std::size_t size) {
        if (size == 0) {
            data_ = nullptr;
            size_ = 0;
        } else {
            data_ = new T[size]();
            size_ = size;
        }
    }

    explicit Vector(std::initializer_list<T> other) {
        if (other.size() == 0) {
            data_ = nullptr;
            size_ = 0;
        } else {
            size_ = other.size();
            data_ = new T[size_];
            std::copy(other.begin(), other.end(), data_);
        }
    }

    explicit Vector(const std::size_t size, const T* other) {
        if (other == nullptr || size == 0) {
            data_ = nullptr;
            size_ = 0;
        } else {
            size_ = size;
            data_ = new T[size];
            std::copy(other, other + size, data_);
        }
    }

    Vector(const Vector& other) {
        if (other.data_ == nullptr || other.size_ == 0) {
            data_ = nullptr;
            size_ = 0;
        } else {
            size_ = other.size_;
            data_ = new T[other.size_];
            std::copy(other.data_, other.data_ + other.size_, data_);
        }
    }

    Vector(Vector&& other) noexcept : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    [[nodiscard]] std::size_t size() const {
        return size_;
    }

    T& operator[](std::size_t index);

    const T& operator[](std::size_t index) const;

    void swap(Vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    // CAS: pass by value handles both copy and move assignment
    Vector& operator=(Vector other) noexcept;

    // Vector& operator=(const Vector& other);

    // Vector& operator=(Vector&& other) noexcept;

    template <typename U>
    friend std::istream& operator>>(std::istream& in, Vector<U>& vector);

    template <typename U>
    friend std::ostream& operator<<(std::ostream& out, const Vector<U>& vector);

    ~Vector() {
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
    }
};

template <typename U>
U& Vector<U>::operator[](const std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Index out of bounds");
    }
    return data_[index];
}

template <typename U>
const U& Vector<U>::operator[](const std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Index out of bounds");
    }
    return data_[index];
}

// CAS: parameter is already a copy (or moved-into), just swap
template <typename U>
Vector<U>& Vector<U>::operator=(Vector other) noexcept {
    other.swap(*this);
    return *this;
}

// template <typename U>
// Vector<U>& Vector<U>::operator=(const Vector& other) {
//     if (this != &other) {
//         if (other.data_ == nullptr || other.size_ == 0) {
//             delete[] data_;
//             data_ = nullptr;
//             size_ = 0;
//         } else {
//             U* new_data = new U[other.size_];
//             std::copy(other.data_, other.data_ + other.size_, new_data);
//             delete[] data_;
//             data_ = new_data;
//             size_ = other.size_;
//         }
//     }
//     return *this;
// }
//
// template <typename U>
// Vector<U>& Vector<U>::operator=(Vector&& other) noexcept {
//     if (this != &other) {
//         delete[] data_;
//         data_ = other.data_;
//         size_ = other.size_;
//         other.data_ = nullptr;
//         other.size_ = 0;
//     }
//     return *this;
// }

template <typename U>
std::istream& operator>>(std::istream& in, Vector<U>& vector) {
    std::size_t capacity = 8;
    U* buffer = new U[capacity];
    std::size_t count = 0;
    for (U temp; in >> temp;) {
        if (count == capacity) {
            capacity *= 2;
            U* new_buffer = new U[capacity];
            std::copy(buffer, buffer + count, new_buffer);
            delete[] buffer;
            buffer = new_buffer;
        }
        buffer[count++] = temp;
    }
    delete[] vector.data_;
    vector.data_ = nullptr;
    vector.size_ = count;
    if (count > 0) {
        vector.data_ = new U[count];
        std::copy(buffer, buffer + count, vector.data_);
    }
    delete[] buffer;
    return in;
}

template <typename U>
std::ostream& operator<<(std::ostream& out, const Vector<U>& vector) {
    for (std::size_t i = 0; i < vector.size_; i++) {
        out << vector[i];
        if (i < vector.size_ - 1) {
            out << " ";
        }
    }
    return out;
}

#endif //CPP_VECTOR_H
