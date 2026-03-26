#ifndef CPP_UNIQUEPTR_H
#define CPP_UNIQUEPTR_H

#include <memory>
#include <utility>

template <typename T>
struct DefaultDeleter {
    void operator()(const T* ptr) const noexcept {
        delete ptr;
    }
};

template <typename T>
struct DefaultDeleter<T[]> {
    void operator()(const T* ptr) const noexcept {
        delete[] ptr;
    }
};

template <typename T, typename Deleter = DefaultDeleter<T>>
class Uniqueptr {
    T* ptr_;
    [[no_unique_address]] Deleter deleter_;

public:
    Uniqueptr() noexcept : ptr_(nullptr), deleter_(Deleter{}) {}

    explicit Uniqueptr(std::nullptr_t) noexcept : ptr_(nullptr), deleter_() {}

    explicit Uniqueptr(T* ptr) noexcept : ptr_(ptr), deleter_(Deleter{}) {}

    Uniqueptr(T* ptr, Deleter deleter) noexcept : ptr_(ptr), deleter_(std::move(deleter)) {}

    Uniqueptr(const Uniqueptr&) = delete;

    Uniqueptr(Uniqueptr&& other) noexcept : ptr_(std::exchange(other.ptr_, nullptr)), deleter_(std::move(other.deleter_)) {}

    template <typename U, typename E,
              typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    Uniqueptr(Uniqueptr<U, E>&& other) noexcept
        : ptr_(other.release()),
          deleter_() {}

    Uniqueptr& operator=(const Uniqueptr&) = delete;

    Uniqueptr& operator=(Uniqueptr&& other) noexcept {
        if (this != &other) {
            if (ptr_) {
                deleter_(ptr_);
            }
            ptr_ = std::exchange(other.ptr_, nullptr);
            deleter_ = std::move(other.deleter_);
        }
        return *this;
    }

    Uniqueptr& operator=(std::nullptr_t) noexcept {
        reset();
        return *this;
    }

    T& operator*() const noexcept {
        return *ptr_;
    }

    T* operator->() const noexcept {
        return ptr_;
    }

    T* get() const noexcept {
        return ptr_;
    }

    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }

    Deleter& get_deleter() noexcept {
        return deleter_;
    }

    const Deleter& get_deleter() const noexcept {
        return deleter_;
    }

    T* release() noexcept {
        return std::exchange(ptr_, nullptr);
    }

    void reset(T* ptr = nullptr) noexcept {
        if (T* old = std::exchange(ptr_, ptr)) {
            deleter_(old);
        }
    }

    void swap(Uniqueptr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(deleter_, other.deleter_);
    }

    friend bool operator==(const Uniqueptr& lhs, const Uniqueptr& rhs) noexcept {
        return lhs.ptr_ == rhs.ptr_;
    }

    friend bool operator!=(const Uniqueptr& lhs, const Uniqueptr& rhs) noexcept {
        return lhs.ptr_ != rhs.ptr_;
    }

    friend bool operator==(const Uniqueptr& lhs, std::nullptr_t) noexcept {
        return lhs.ptr_ == nullptr;
    }

    friend bool operator!=(const Uniqueptr& lhs, std::nullptr_t) noexcept {
        return lhs.ptr_ != nullptr;
    }

    friend bool operator==(std::nullptr_t, const Uniqueptr& rhs) noexcept {
        return rhs.ptr_ == nullptr;
    }

    friend bool operator!=(std::nullptr_t, const Uniqueptr& rhs) noexcept {
        return rhs.ptr_ != nullptr;
    }

    ~Uniqueptr() noexcept {
        if (ptr_) {
            deleter_(ptr_);
        }
    }
};

template <typename T, typename... Args>
Uniqueptr<T> MakeUnique(Args&&... args) {
    return Uniqueptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename Deleter>
class Uniqueptr<T[], Deleter> {
    T* ptr_;
    [[no_unique_address]] Deleter deleter_;

public:
    Uniqueptr() noexcept : ptr_(nullptr), deleter_(Deleter{}) {}

    explicit Uniqueptr(std::nullptr_t) noexcept : ptr_(nullptr), deleter_() {}

    explicit Uniqueptr(T* ptr) noexcept : ptr_(ptr), deleter_(Deleter{}) {}

    Uniqueptr(T* ptr, Deleter deleter) noexcept : ptr_(ptr), deleter_(std::move(deleter)) {}

    Uniqueptr(const Uniqueptr&) = delete;

    Uniqueptr(Uniqueptr&& other) noexcept : ptr_(std::exchange(other.ptr_, nullptr)), deleter_(std::move(other.deleter_)) {}

    Uniqueptr& operator=(const Uniqueptr&) = delete;

    Uniqueptr& operator=(Uniqueptr&& other) noexcept {
        if (this != &other) {
            if (ptr_) {
                deleter_(ptr_);
            }
            ptr_ = std::exchange(other.ptr_, nullptr);
            deleter_ = std::move(other.deleter_);
        }
        return *this;
    }

    T& operator[](std::size_t index) noexcept {
        return ptr_[index];
    }

    const T& operator[](std::size_t index) const noexcept {
        return ptr_[index];
    }

    T* get() const noexcept {
        return ptr_;
    }

    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }

    Deleter& get_deleter() noexcept {
        return deleter_;
    }

    const Deleter& get_deleter() const noexcept {
        return deleter_;
    }

    T* release() noexcept {
        return std::exchange(ptr_, nullptr);
    }

    void reset(T* ptr = nullptr) noexcept {
        if (T* old = std::exchange(ptr_, ptr)) {
            deleter_(old);
        }
    }

    void swap(Uniqueptr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(deleter_, other.deleter_);
    }

    friend bool operator==(const Uniqueptr& lhs, const Uniqueptr& rhs) noexcept {
        return lhs.ptr_ == rhs.ptr_;
    }

    friend bool operator!=(const Uniqueptr& lhs, const Uniqueptr& rhs) noexcept {
        return lhs.ptr_ != rhs.ptr_;
    }

    friend bool operator==(const Uniqueptr& lhs, std::nullptr_t) noexcept {
        return lhs.ptr_ == nullptr;
    }

    friend bool operator!=(const Uniqueptr& lhs, std::nullptr_t) noexcept {
        return lhs.ptr_ != nullptr;
    }

    friend bool operator==(std::nullptr_t, const Uniqueptr& rhs) noexcept {
        return rhs.ptr_ == nullptr;
    }

    friend bool operator!=(std::nullptr_t, const Uniqueptr& rhs) noexcept {
        return rhs.ptr_ != nullptr;
    }

    ~Uniqueptr() noexcept {
        if (ptr_) {
            deleter_(ptr_);
        }
    }
};

template <typename T>
Uniqueptr<T[]> MakeUniqueArray(const std::size_t size) {
    return Uniqueptr<T[]>(new T[size]());
}

#endif //CPP_UNIQUEPTR_H
