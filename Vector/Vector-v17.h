#ifndef CPP_VECTOR_V17_H
#define CPP_VECTOR_V17_H

#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <algorithm>
#include <utility>
#include <iterator>

template <typename T>
class Vector {
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

public:
    Vector() : data_(nullptr), size_(0), capacity_(0) {}

    explicit Vector(const std::size_t size) {
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

    explicit Vector(std::initializer_list<T> other) {
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

    explicit Vector(const std::size_t size, const T* other) {
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

    Vector(const Vector& other) {
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

    Vector(Vector&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return size_;
    }

    [[nodiscard]] std::size_t capacity() const noexcept {
        return capacity_;
    }

    void reserve(std::size_t new_cap);

    void push_back(const T& val);

    void push_back(T&& val);

    template <typename... Args>
    T& emplace_back(Args&&... args);

    void pop_back();

    void resize(std::size_t count);

    void resize(std::size_t count, const T& val);

    void assign(std::size_t count, const T& val);

    T* insert(const T* pos, const T& val);

    T* erase(const T* pos);

    void shrink_to_fit();

    T& operator[](std::size_t index);

    const T& operator[](std::size_t index) const;

    T* begin() noexcept {
        return data_;
    }

    T* end() noexcept {
        return data_ + size_;
    }

    const T* begin() const noexcept {
        return data_;
    }

    const T* end() const noexcept {
        return data_ + size_;
    }

    std::reverse_iterator<T*> rbegin() noexcept {
        return std::reverse_iterator<T*>(end());
    }

    std::reverse_iterator<T*> rend() noexcept {
        return std::reverse_iterator<T*>(begin());
    }

    std::reverse_iterator<const T*> rbegin() const noexcept {
        return std::reverse_iterator<const T*>(end());
    }

    std::reverse_iterator<const T*> rend() const noexcept {
        return std::reverse_iterator<const T*>(begin());
    }

    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }

    void clear() {
        size_ = 0;
    }

    T* data() noexcept {
        return data_;
    }

    const T* data() const noexcept {
        return data_;
    }

    void swap(Vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    Vector& operator=(Vector other) noexcept;

    // Vector& operator=(const Vector& other);

    // Vector& operator=(Vector&& other) noexcept;

    template <typename U>
    friend std::istream& operator>>(std::istream& in, Vector<U>& vector);

    template <typename U>
    friend std::ostream& operator<<(std::ostream& out, const Vector<U>& vector);

    ~Vector() {
        delete[] data_;
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
//             capacity_ = 0;
//         } else {
//             U* new_data = new U[other.size_];
//             std::copy(other.data_, other.data_ + other.size_, new_data);
//             delete[] data_;
//             data_ = new_data;
//             size_ = other.size_;
//             capacity_ = size_;
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
//         capacity_ = size_;
//         other.data_ = nullptr;
//         other.size_ = 0;
//     }
//     return *this;
// }

template <typename U>
void Vector<U>::reserve(const std::size_t new_cap) {
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

template <typename U>
void Vector<U>::push_back(const U& val) {
    if (size_ == capacity_) {
        U copy = val;
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        data_[size_++] = std::move(copy);
    } else {
        data_[size_++] = val;
    }
}

template <typename U>
void Vector<U>::push_back(U&& val) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    data_[size_++] = std::move(val);
}

template <typename U>
template <typename... Args>
U& Vector<U>::emplace_back(Args&&... args) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    data_[size_] = U(std::forward<Args>(args)...);
    return data_[size_++];
}

template <typename U>
void Vector<U>::pop_back() {
    if (size_ > 0) {
        --size_;
        data_[size_] = U();
    }
}

template <typename U>
void Vector<U>::resize(const std::size_t count) {
    if (count > capacity_) {
        reserve(count);
    }
    for (std::size_t i = size_; i < count; ++i) {
        data_[i] = U();
    }
    size_ = count;
}

template <typename U>
void Vector<U>::resize(const std::size_t count, const U& val) {
    if (count > capacity_) {
        reserve(count);
    }
    for (std::size_t i = size_; i < count; ++i) {
        data_[i] = val;
    }
    size_ = count;
}

template <typename U>
void Vector<U>::assign(const std::size_t count, const U& val) {
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

template <typename U>
U* Vector<U>::insert(const U* pos, const U& val) {
    const std::size_t index = pos - data_;
    if (index > size_) {
        throw std::out_of_range("Insert position out of bounds");
    }
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    for (std::size_t i = size_; i > index; --i) {
        data_[i] = std::move(data_[i - 1]);
    }
    data_[index] = val;
    ++size_;
    return data_ + index;
}

template <typename U>
U* Vector<U>::erase(const U* pos) {
    const std::size_t index = pos - data_;
    if (index >= size_) {
        throw std::out_of_range("Erase position out of bounds");
    }
    for (std::size_t i = index; i < size_ - 1; ++i) {
        data_[i] = std::move(data_[i + 1]);
    }
    --size_;
    data_[size_] = U();
    return data_ + index;
}

template <typename U>
void Vector<U>::shrink_to_fit() {
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

#endif //CPP_VECTOR_V17_H
