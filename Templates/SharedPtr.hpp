#pragma once

//source - https://github.com/GeorgiTerziev02/Object-oriented_programming_FMI/tree/main/Sem.%2013/Solutions/SharedPointer

template<typename T>
class SharedPtr {
private:
    T* data = nullptr;
    size_t* counter = nullptr;

public:
    SharedPtr(T* data = nullptr);
    SharedPtr(const SharedPtr<T>& other);
    SharedPtr(SharedPtr<T>&& other) noexcept;
    SharedPtr<T>& operator=(const SharedPtr<T>& other);
    SharedPtr<T>& operator=(SharedPtr<T>&& other) noexcept;
    ~SharedPtr();

    const T* operator->() const;
    T* operator->();
    const T& operator*() const;
    T& operator*();

    T* get() const;
    void reset(T* data = nullptr);
    size_t getCount() const;

    explicit operator bool() const;

private:
    void copyFrom(const SharedPtr<T>& other);
    void moveFrom(SharedPtr<T>&& other) noexcept;
    void free();
};


template<typename T>
SharedPtr<T>::SharedPtr(T* data) {
    this->data = data;
    if (this->data) {
        counter = new size_t(1);
    }
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other) {
    copyFrom(other);
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& other) noexcept {
    moveFrom(std::move(other));
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other) {
    if (this != &other) {
        free();
        copyFrom(other);
    }

    return *this;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& other) noexcept {
    if (this != &other) {
        free();
        moveFrom(std::move(other));
    }

    return *this;
}

template<typename T>
SharedPtr<T>::~SharedPtr() {
    free();
}

template<typename T>
const T* SharedPtr<T>::operator->() const {
    return data;
}

template<typename T>
T* SharedPtr<T>::operator->() {
    return data;
}

template<typename T>
const T& SharedPtr<T>::operator*() const {
    return *data;
}

template<typename T>
T& SharedPtr<T>::operator*() {
    return *data;
}

template<typename T>
T* SharedPtr<T>::get() const {
    return data;
}

template<typename T>
void SharedPtr<T>::reset(T* dataPtr) {
    if (this->data == dataPtr) {
        return;
    }

    free();

    this->data = dataPtr;
    if (this->data) {
        counter = new size_t(1);
    }
}

template<typename T>
size_t SharedPtr<T>::getCount() const {
    return *counter;
}

template<typename T>
SharedPtr<T>::operator bool() const {
    return data;
}

template<typename T>
void SharedPtr<T>::copyFrom(const SharedPtr<T>& other) {
    data = other.data;
    counter = other.counter;
    if (data) {
        (*counter)++;
    }
}

template<typename T>
void SharedPtr<T>::moveFrom(SharedPtr<T>&& other) noexcept {
    data = other.data;
    counter = other.counter;

    other.data = nullptr;
    other.counter = nullptr;
}

template<typename T>
void SharedPtr<T>::free() {
    if (!data) {
        return;
    }

    if ((*counter) == 1) {
        delete data;
        delete counter;
    }
    else {
        (*counter)--;
    }
}
