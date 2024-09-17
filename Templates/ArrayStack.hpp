#include <istream>

template<typename T>
class ArrayStack{
private:
    T* data;
    size_t currentSize;
    size_t capacity;

    void copyFrom(const ArrayStack<T>& other);
    void free();
    void resize(size_t newCapacity);

public:
    ArrayStack();
    ArrayStack(const ArrayStack<T>& other);
    ArrayStack<T>& operator=(const ArrayStack<T>& other);
    ~ArrayStack();

    void push(const T& obj);
    T pop();
    const T& peek() const;

    size_t getSize() const;
    size_t isEmpty() const;
};

template<typename T>
ArrayStack<T>::ArrayStack() : currentSize(0), capacity(4) {
    data = new T[capacity];
}

template<typename T>
ArrayStack<T>::ArrayStack(const ArrayStack<T>& other) {
    copyFrom(other);
}

template<typename T>
ArrayStack<T>& ArrayStack<T>::operator=(const ArrayStack<T>& other) {
    if(this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

template<typename T>
ArrayStack<T>::~ArrayStack() {
    free();
}

template<typename T>
void ArrayStack<T>::copyFrom(const ArrayStack<T>& other) {
    data = new T[other.capacity];

    for(size_t i = 0; i < other.currentSize; ++i) {
        data[i] = other.data[i];
    }

    currentSize = other.currentSize;
    capacity = other.capacity;
}

template<typename T>
void ArrayStack<T>::free() {
    delete[] data;
}

template<typename T>
void ArrayStack<T>::resize(size_t newCapacity) {
    T* temp = data;
    data = new T[newCapacity];

    for(size_t i = 0; i < currentSize; i++) {
        data[i] = temp[i];
    }

    capacity = newCapacity;
    delete[] temp;
}

template<typename T>
void ArrayStack<T>::push(const T& obj) {

    if(currentSize >= capacity) {
        resize(capacity * 2);
    }

    data[currentSize++] = obj;
}

template<typename T>
T ArrayStack<T>::pop() {
    if(isEmpty()) {
        throw std::runtime_error("Stack is empty!");
    }

    T obj = data[--currentSize];

    if(currentSize * 2 <= capacity && capacity > 1) {
        resize(capacity / 2);
    }

    return obj;
}

template<typename T>
const T& ArrayStack<T>::peek() const {
    if(isEmpty()) {
        throw std::runtime_error("Stack is empty!");
    }

    return data[currentSize - 1];
}

template<typename T>
size_t ArrayStack<T>::getSize() const {
    return currentSize;
}

template<typename T>
size_t ArrayStack<T>::isEmpty() const {
    return currentSize == 0;
}
