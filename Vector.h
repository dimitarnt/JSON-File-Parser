#pragma once
#include <iostream>
#include <exception>

//source - https://github.com/GeorgiTerziev02/Object-oriented_programming_FMI/tree/main/Sem.%2008/Solutions/Vector

namespace {
    const short INITIAL_CAPACITY = 4;
    const short RESIZE_MULTIPLIER = 2;
}

template <typename T>
class Vector {
private:
    T* data = nullptr;
    size_t size = 0;
    size_t capacity;

    void assertIndex(size_t index) const;

    void copyFrom(const Vector& other);
    void moveFrom(Vector&& other);
    void free();

    void resize(size_t newCapacity);
    void upsizeIfNeeded();
    void downsizeIfNeeded();

public:
    Vector();
    Vector(size_t capacity);
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;
    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;
    ~Vector();

    size_t getSize() const;
    size_t getCapacity() const;

    void pushBack(const T& element);
    void pushAt(const T& element, size_t index);
    T popBack();
    T popAt(size_t index);

    bool empty() const;
    void clear();
    void shrinkToFit();

    T& operator[](size_t index);
    const T& operator[](size_t index) const;
};


template <typename T>
Vector<T>::Vector() : Vector(INITIAL_CAPACITY) { }

template <typename T>
Vector<T>::Vector(size_t capacity) : capacity(capacity) {
    data = new T[capacity];
}

template <typename T>
Vector<T>::Vector(const Vector& other) {
    copyFrom(other);
}

template <typename T>
Vector<T>::Vector(Vector<T>&& other) noexcept {
    moveFrom(std::move(other));
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (this != &other) {
        copyFrom(other);
        free();
    }

    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept {
    if (this != &other) {
        moveFrom(std::move(other));
        free();
    }

    return *this;
}

template <typename T>
Vector<T>::~Vector() {
    free();
}

template <typename T>
void Vector<T>::assertIndex(size_t index) const {
    if (index >= size) {
        throw std::out_of_range("Error, index is out of range");
    }
}

template <typename T>
void Vector<T>::upsizeIfNeeded() {
    if (size == capacity) {
        resize(capacity * RESIZE_MULTIPLIER);
    }
}

template <typename T>
void Vector<T>::downsizeIfNeeded() {
    if (size * RESIZE_MULTIPLIER * RESIZE_MULTIPLIER <= capacity) {
        resize(capacity / RESIZE_MULTIPLIER);
    }
}

template <typename T>
void Vector<T>::resize(size_t newCapacity) {
    capacity = newCapacity;
    T* temp = new T[capacity];

    // Note: the std::vector allocates ONLY MORE than the current capacity
    if (size > newCapacity) {
        size = newCapacity;
    }

    for (size_t i = 0; i < size; ++i) {
        temp[i] = data[i];
    }

    delete[] data;
    data = temp;
}

template <typename T>
size_t Vector<T>::getSize() const {
    return size;
}

template <typename T>
size_t Vector<T>::getCapacity() const {
    return capacity;
}

template <typename T>
void Vector<T>::pushBack(const T& element) {
    upsizeIfNeeded();
    data[size++] = element;
}

template <typename T>
void Vector<T>::pushAt(const T& element, size_t index) {
    assertIndex(index);
    upsizeIfNeeded();

    for (size_t i = size; i > index; --i) {
        data[i] = data[i - 1];
    }

    data[index] = element;
    size++;
}

template <typename T>
T Vector<T>::popBack() {
    if (empty()) {
        throw std::length_error("Vector is empty");
    }
    // Note: the actual std::vector does NOT lower its capacity on this function
    downsizeIfNeeded();
    // Note: the actual std::vector does NOT return on popBack
    return data[--size];
}

template <typename T>
T Vector<T>::popAt(size_t index) {
    assertIndex(index);
    // Note: the actual std::vector does NOT lower its capacity on this function
    downsizeIfNeeded();

    T temp = data[index];
    size--;
    for (size_t i = index; i < size; ++i) {
        data[i] = data[i + 1];
    }

    return temp;
}

template <typename T>
bool Vector<T>::empty() const {
    return size == 0;
}

template <typename T>
void Vector<T>::clear() {
    size = 0;
}

template <typename T>
void Vector<T>::shrinkToFit() {
    resize(size);
}

template <typename T>
T& Vector<T>::operator[](size_t index) {
    assertIndex(index);
    return data[index];
}

template <typename T>
const T& Vector<T>::operator[](size_t index) const {
    assertIndex(index);
    return data[index];
}

template <typename T>
void Vector<T>::copyFrom(const Vector& other) {
    size = other.size;
    capacity = other.capacity;
    data = new T[capacity];

    for (size_t i = 0; i < other.size; ++i) {
        data[i] = other.data[i];
    }
}

template <typename T>
void Vector<T>::moveFrom(Vector<T>&& other) {
    size = other.size;
    capacity = other.capacity;
    data = other.data;
    other.data = nullptr;
}

template <typename T>
void Vector<T>::free() {
    delete[] data;
    data = nullptr;
    size = capacity = 0;
}
