#pragma once
#include "constants.h"
#include "helperFunctions.h"
#include <iostream>

const int MAX_SMALL_STRING_LENGTH = NUMBER_OF_BYTES_IN_CHAR_PTR + NUMBER_OF_BYTES_IN_SIZE_T - 1;
const long long MAX_STRING_LENGTH = NUMBER_OF_BYTES_IN_SIZE_T == 4 ? pow(2, 31) - 1 : pow(2, 63) - 1;

const char* const DEFAULT_STRING = "\0";

class String {
    union {
        char _smallString[MAX_SMALL_STRING_LENGTH + 1] = {};
        struct {
            char* _data;
            size_t _inflatedLength;
        };
    };

    size_t longStringLength() const;

    void copyFrom(const String& other);
    void moveFrom(String&& other);
    void free();

    void assertLength(size_t length) const;

    bool canBeOptimized(size_t strLength) const;
    bool isOptimized() const;

public:
    String();
    String(const char* data);
    String(const String& other);
    String(String&& other) noexcept ;
    String& operator=(const String& other);
    String& operator=(String&& other) noexcept ;
    ~String();

    void setMembers(const char* data);
    const char* getData() const;
    size_t getLength() const;

    String substring(size_t begin, size_t substringLength) const;
    String& operator+=(const String& other);
    String& operator+=(char symbol);
    String& operator+=(size_t number);
    String& operator+=(unsigned number);

    char& operator[](size_t index);
    char operator[](size_t index) const;

    friend String operator+(const String& lhs, const String& rhs);
    friend String operator+(const String& lhs, char symbol);
    friend String operator+(const String& lhs, size_t number);
    friend String operator+(const String& lhs, unsigned number);
    friend std::istream& operator>>(std::istream&, String& str);
};

String operator+(const String& lhs, const String& rhs);
String operator+(const String& lhs, char symbol);
String operator+(const String& lhs, size_t number);
String operator+(const String& lhs, unsigned number);

std::ostream& operator<<(std::ostream& os, const String& str);

std::istream& operator>>(std::istream& is, String& str);

bool operator<(const String& lhs, const String& rhs);
bool operator<=(const String& lhs, const String& rhs);
bool operator>=(const String& lhs, const String& rhs);
bool operator>(const String& lhs, const String& rhs);
bool operator==(const String& lhs, const String& rhs);
bool operator!=(const String& lhs, const String& rhs);
