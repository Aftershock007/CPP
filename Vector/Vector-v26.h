#ifndef CPP_VECTOR_V26_H
#define CPP_VECTOR_V26_H

#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <algorithm>
#include <utility>
#include <iterator>
#include <concepts>

template <typename T>
class Vector {
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

public:
    constexpr Vector() : data_(nullptr), size_(0), capacity_(0) {}

    constexpr explicit Vector(const std::size_t size) {
        if (size == 0) {
            data_ = nullptr;
            size_ = 0;
            capacity_ = 0;
        } else {
            data_ = new T[size]();
            size_ = size;
            capacity_ = size;
        }
    }

    constexpr explicit Vector(std::initializer_list<T> other) {
        if (other.size() == 0) {
            data_ = nullptr;
            size_ = 0;
            capacity_ = 0;
        } else {
            size_ = other.size();
            data_ = new T[size_];
            capacity_ = size_;
            std::copy(other.begin(), other.end(), data_);
        }
    }

    constexpr explicit Vector(const std::size_t size, const T* other) {
        if (other == nullptr || size == 0) {
            data_ = nullptr;
            size_ = 0;
            capacity_ = 0;
        } else {
            size_ = size;
            capacity_ = size;
            data_ = new T[size];
            std::copy(other, other + size, data_);
        }
    }

    constexpr Vector(const Vector& other) {
        if (other.data_ == nullptr || other.size_ == 0) {
            data_ = nullptr;
            size_ = 0;
            capacity_ = 0;
        } else {
            size_ = other.size_;
            capacity_ = other.size_;
            data_ = new T[other.size_];
            std::copy(other.data_, other.data_ + other.size_, data_);
        }
    }

    constexpr Vector(Vector&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    [[nodiscard]] constexpr std::size_t size() const noexcept {
        return size_;
    }

    [[nodiscard]] constexpr std::size_t capacity() const noexcept {
        return capacity_;
    }

    constexpr void reserve(std::size_t new_cap);

    constexpr void push_back(const T& val);

    constexpr void push_back(T&& val);

    template <typename... Args>
        requires std::constructible_from<T, Args...>
    constexpr T& emplace_back(Args&&... args);

    constexpr void pop_back();

    constexpr void resize(std::size_t count);

    constexpr void resize(std::size_t count, const T& val);

    constexpr void assign(std::size_t count, const T& val);

    constexpr T* insert(const T* pos, const T& val);

    constexpr T* erase(const T* pos);

    constexpr void shrink_to_fit();

    // Deducing this (C++23) — single overload handles const and non-const access
    constexpr auto&& operator[](this auto&& self, std::size_t index) {
        if (index >= self.size_) {
            throw std::out_of_range("Index out of bounds");
        }
        return std::forward_like<decltype(self)>(self.data_[index]);
    }

    constexpr T* begin() noexcept {
        return data_;
    }

    constexpr T* end() noexcept {
        return data_ + size_;
    }

    constexpr const T* begin() const noexcept {
        return data_;
    }

    constexpr const T* end() const noexcept {
        return data_ + size_;
    }

    constexpr std::reverse_iterator<T*> rbegin() noexcept {
        return std::reverse_iterator<T*>(end());
    }

    constexpr std::reverse_iterator<T*> rend() noexcept {
        return std::reverse_iterator<T*>(begin());
    }

    constexpr std::reverse_iterator<const T*> rbegin() const noexcept {
        return std::reverse_iterator<const T*>(end());
    }

    constexpr std::reverse_iterator<const T*> rend() const noexcept {
        return std::reverse_iterator<const T*>(begin());
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return size_ == 0;
    }

    constexpr void clear() {
        size_ = 0;
    }

    constexpr T* data() noexcept {
        return data_;
    }

    constexpr const T* data() const noexcept {
        return data_;
    }

    constexpr void swap(Vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    constexpr Vector& operator=(Vector other) noexcept;

    template <typename U>
    friend std::istream& operator>>(std::istream& in, Vector<U>& vector);

    template <typename U>
    friend std::ostream& operator<<(std::ostream& out, const Vector<U>& vector);

    constexpr ~Vector() {
        delete[] data_;
    }
};

// Strong exception guarantee: if copy throws, 'other' was never created.
// Nothrow: swap itself is noexcept.
template <typename U>
constexpr Vector<U>& Vector<U>::operator=(Vector other) noexcept {
    other.swap(*this);
    return *this;
}

// Strong exception guarantee: if new throws, state is unchanged.
// Uses std::move_if_noexcept for strong exception guarantee when relocating elements.
template <typename U>
constexpr void Vector<U>::reserve(const std::size_t new_cap) {
    if (new_cap <= capacity_) {
        return;
    }
    U* new_data = new U[new_cap];
    if (data_ != nullptr) {
        for (std::size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move_if_noexcept(data_[i]);
        }
    }
    delete[] data_;
    data_ = new_data;
    capacity_ = new_cap;
}

// Strong exception guarantee: if reserve throws, state is unchanged.
// Copies val before reserve to handle self-reference (e.g. vec.push_back(vec[0])).
template <typename U>
constexpr void Vector<U>::push_back(const U& val) {
    if (size_ == capacity_) {
        U copy = val;
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        data_[size_++] = std::move(copy);
    } else {
        data_[size_++] = val;
    }
}

// Strong exception guarantee. No self-reference risk with rvalues.
template <typename U>
constexpr void Vector<U>::push_back(U&& val) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    data_[size_++] = std::move(val);
}

// Strong exception guarantee.
template <typename U>
template <typename... Args>
    requires std::constructible_from<U, Args...>
constexpr U& Vector<U>::emplace_back(Args&&... args) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    data_[size_] = U(std::forward<Args>(args)...);
    return data_[size_++];
}

// Nothrow guarantee.
template <typename U>
constexpr void Vector<U>::pop_back() {
    if (size_ > 0) {
        --size_;
        data_[size_] = U();
    }
}

template <typename U>
constexpr void Vector<U>::resize(const std::size_t count) {
    if (count > capacity_) {
        reserve(count);
    }
    // Value-initialize new elements if growing
    for (std::size_t i = size_; i < count; ++i) {
        data_[i] = U();
    }
    size_ = count;
}

template <typename U>
constexpr void Vector<U>::resize(const std::size_t count, const U& val) {
    if (count > capacity_) {
        reserve(count);
    }
    // Fill new elements with val if growing
    for (std::size_t i = size_; i < count; ++i) {
        data_[i] = val;
    }
    size_ = count;
}

// Strong exception guarantee: allocate before delete.
template <typename U>
constexpr void Vector<U>::assign(const std::size_t count, const U& val) {
    if (count > capacity_) {
        U* new_data = new U[count];
        delete[] data_;
        data_ = new_data;
        capacity_ = count;
    }
    size_ = count;
    for (std::size_t i = 0; i < count; ++i) {
        data_[i] = val;
    }
}

// Basic exception guarantee. Invalidates iterators if reallocation occurs.
template <typename U>
constexpr U* Vector<U>::insert(const U* pos, const U& val) {
    const std::size_t index = pos - data_;
    if (index > size_) {
        throw std::out_of_range("Insert position out of bounds");
    }
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    // Shift elements right
    for (std::size_t i = size_; i > index; --i) {
        data_[i] = std::move(data_[i - 1]);
    }
    data_[index] = val;
    ++size_;
    return data_ + index;
}

// Basic exception guarantee. Invalidates iterators at and after pos.
template <typename U>
constexpr U* Vector<U>::erase(const U* pos) {
    const std::size_t index = pos - data_;
    if (index >= size_) {
        throw std::out_of_range("Erase position out of bounds");
    }
    // Shift elements left
    for (std::size_t i = index; i < size_ - 1; ++i) {
        data_[i] = std::move(data_[i + 1]);
    }
    --size_;
    data_[size_] = U();
    return data_ + index;
}

// Strong exception guarantee: if new throws, state is unchanged.
template <typename U>
constexpr void Vector<U>::shrink_to_fit() {
    if (capacity_ == size_) {
        return;
    }
    if (size_ == 0) {
        delete[] data_;
        data_ = nullptr;
        capacity_ = 0;
        return;
    }
    U* new_data = new U[size_];
    std::copy(data_, data_ + size_, new_data);
    delete[] data_;
    data_ = new_data;
    capacity_ = size_;
}

template <typename U>
std::istream& operator>>(std::istream& in, Vector<U>& vector) {
    vector.clear();
    for (U temp; in >> temp;) {
        vector.push_back(std::move(temp));
    }
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

#endif //CPP_VECTOR_V26_H
