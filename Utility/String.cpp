#include "String.h"
#include "constants.h"
#include <cstring>

String::String() : String(DEFAULT_STRING) {}

String::String(const char* data) {
    setMembers(data);
}

String::String(const String& other) {
    copyFrom(other);
}

String::String(String &&other) noexcept {
    moveFrom(std::move(other));
}

String& String::operator=(const String& other) {
    if(this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

String& String::operator=(String&& other) noexcept {
    if(this != &other) {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

String::~String() {
    free();
}

void String::copyFrom(const String& other) {
    if(other.isOptimized()) {
        strcpy(_smallString, other._smallString);
        _smallString[MAX_SMALL_STRING_LENGTH] = 0;

    }else {
        _inflatedLength = other._inflatedLength;
        _data = new char[other.longStringLength() + 1];
        strcpy(_data, other._data);
    }
}

void String::moveFrom(String&& other) {
    if(other.isOptimized()) {
        strcpy(_smallString, other._smallString);
        _smallString[MAX_SMALL_STRING_LENGTH] = 0;

    }else {
        _inflatedLength = other._inflatedLength;
        _data = other._data;
        other._data = nullptr;
    }
}

void String::free() {
    if(!isOptimized()) {
        delete[] _data;
    }
    _data = nullptr;
    _inflatedLength = 0;
}

void String::assertData(const char* data) {
    if (data == nullptr) {
        throw std::invalid_argument("Error, data is nullptr");
    }
}

void String::assertLength(size_t length) {
    if(length > MAX_STRING_LENGTH) {
        throw std::invalid_argument("Error, data is too long");
    }
}

void String::assertIndex(size_t index) const {
    if(index >= getLength()) {
        throw std::out_of_range("Error, index is out of range");
    }
}

size_t String::longStringLength() const {

    return _inflatedLength - MAX_STRING_LENGTH - 2;
}

void String::setMembers(const char* data) {
    assertData(data);

    size_t dataLength = strlen(data);
    assertLength(dataLength);

    if(!isOptimized()) {
        delete[] _data;
    }

    if(canBeOptimized(dataLength)) {

        strcpy(_smallString, data);

        _smallString[MAX_SMALL_STRING_LENGTH] = 0;

    }else {

        ((char*)&dataLength)[NUMBER_OF_BYTES_IN_SIZE_T - 1] |= BITS[0];

        _inflatedLength = dataLength;
        _data = new char[longStringLength() + 1];
        strcpy(_data, data);
    }
}

bool String::canBeOptimized(size_t strLength) const {

    return strLength <= MAX_SMALL_STRING_LENGTH;
}

bool String::isOptimized() const {

    return (((char*)&_inflatedLength)[NUMBER_OF_BYTES_IN_SIZE_T - 1] & BITS[0]) == 0;
}

const char* String::getData() const {
    if(isOptimized()) {
        return _smallString;
    }
    else {
        return _data;
    }
}

size_t String::getLength() const {
    if(isOptimized()) {
        return strlen(_smallString);
    }
    else {
        return longStringLength();
    }
}

void String::clear() {
    *this = "";
}

char& String::operator[](size_t index) {
    assertIndex(index);

    if(isOptimized()) {
        return _smallString[index];
    }
    else {
        return _data[index];
    }
}

char String::operator[](size_t index) const {
    assertIndex(index);

    if(isOptimized()) {
        return _smallString[index];
    }
    else {
        return _data[index];
    }
}

bool String::isNumber() const {
    size_t currentIndex = 0;
    size_t numberOfDigits = getLength();
    bool decimalPointMet = false;

    if(isEmpty()) {
        return false;
    }

    if((*this)[currentIndex] == '-') {

        if((*this) == "-0") {
            return false;
        }

        currentIndex++;
    }

    if((*this)[currentIndex] == '.' || (*this)[numberOfDigits - 1] == '.') {
        return false;
    }

    if(numberOfDigits > 1 && (*this)[currentIndex] == '0' && (*this)[currentIndex + 1] != '.') {
        return false;
    }

    while(currentIndex < numberOfDigits) {
        char currentDigit = (*this)[currentIndex];

        if(!isDigit(currentDigit) && currentDigit != '.') {
            return false;
        }

        if(currentDigit == '.') {

            if(decimalPointMet) {
                return false;
            }

            decimalPointMet = true;
        }

        currentIndex++;
    }

    return true;
}

bool String::isNaturalNumber() const {
    size_t currentLength = getLength();

    if(currentLength == 0) {
        return false;
    }

    if((*this)[0] == '0' && currentLength > 1) {
        return false;
    }

    for(size_t i = 0; i < currentLength; ++i) {

        if(!isDigit((*this)[i])) {
            return false;
        }
    }

    return true;
}

bool String::isEmpty() const {

    return getLength() == 0;
}

String String::substring(size_t begin, size_t substringLength) const {
    if(begin + substringLength > getLength()) {
        throw std::length_error("Error, substring is out of range");
    }

    if(canBeOptimized(substringLength)) {

        if(isOptimized()) {
            char str[MAX_SMALL_STRING_LENGTH + 1] = {};

            for (size_t i = 0; i < substringLength; i++) {
                str[i] = _smallString[begin + i];
            }
            str[substringLength] = '\0';

            String result(str);

            return result;

        }else {
            char str[MAX_SMALL_STRING_LENGTH + 1] = {};

            for (size_t i = 0; i < substringLength; i++) {
                str[i] = _data[begin + i];
            }
            str[substringLength] = '\0';

            String result(str);

            return result;
        }

    }else {
        char* str = new char[substringLength + 1] {};

        for(size_t i = 0; i < substringLength; i++) {
            str[i] = _data[begin + i];
        }
        str[substringLength] = '\0';

        String result(str);

        return result;
    }
}

String String::getFormattedWord(size_t wordPlace) const {
    size_t strLength = getLength();

    size_t wordStart = 0;
    bool wordHasStarted = false;

    for(size_t i = 0; i <= strLength; ++i) {
        if(i == strLength || (*this)[i] == ' ' || (*this)[i] == '\t' || (*this)[i] == '\n') {
            if(wordHasStarted) {
                wordPlace--;
            }

            if(wordPlace == 0) {
                return substring(wordStart, i - wordStart);
            }

            wordHasStarted = false;

            continue;
        }

        if(!wordHasStarted) {
            wordStart = i;
            wordHasStarted = true;
        }
    }

    return "";
}

unsigned String::getCharCount(char symbol) const {
    size_t strLength = getLength();
    size_t count = 0;

    for(unsigned i = 0; i < strLength; ++i) {
        if((*this)[i] == symbol) {
            count++;
        }
    }

    return count;
}

long long String::getPositionOfChar(char symbol, long long timesMet) const {
    size_t strLength = getLength();

    for(unsigned i = 0; i < strLength; ++i) {
        if((*this)[i] == symbol) {
            timesMet--;
        }

        if(timesMet == 0) {
            return i;
        }
    }

    return -1;
}

String& String::operator+=(const String& other) {
    size_t concatenatedLength = this->getLength() + other.getLength();

    if(canBeOptimized(concatenatedLength)) {
        char str[MAX_SMALL_STRING_LENGTH + 1] = {};

        strcpy(str, getData());
        strcat(str, other.getData());

        setMembers(str);

        return *this;

    }else {
        char* str = new char[concatenatedLength + 1] {};

        strcpy(str, getData());
        strcat(str, other.getData());

        setMembers(str);

        return *this;
    }
}

String& String::operator+=(const char* other) {
    assertData(other);

    size_t concatenatedLength = this->getLength() + strlen(other);

    if(canBeOptimized(concatenatedLength)) {
        char str[MAX_SMALL_STRING_LENGTH + 1] = {};

        strcpy(str, getData());
        strcat(str, other);

        setMembers(str);

        return *this;

    }else {
        char* str = new char[concatenatedLength + 1] {};

        strcpy(str, getData());
        strcat(str, other);

        setMembers(str);

        return *this;
    }
}

String& String::operator+=(char symbol) {
    String symbolStr(" ");
    symbolStr[0] = symbol;

    return operator+=(symbolStr);
}

String& String::operator+=(size_t number) {
    size_t digitCount = numberOfDigits(number);
    char temp[MAX_SIZE_T_DIGITS + 1] = {};

    for(size_t i = 0; i < digitCount; ++i) {
        temp[i] = getDigit(number, i, digitCount);
    }

    String numStr(temp);

    return operator+=(numStr);
}

String& String::operator+=(unsigned number) {
    size_t convertedNumber = number;

    return operator+=(convertedNumber);
}

String operator+(const String& lhs, const String& rhs) {
    size_t concatenatedLength = lhs.getLength() + rhs.getLength();

    if(lhs.canBeOptimized(concatenatedLength)) {
        char str[MAX_SMALL_STRING_LENGTH + 1] = {};

        strcpy(str, lhs.getData());
        strcat(str, rhs.getData());

        String result(str);

        return result;

    }else {
        char* str = new char[concatenatedLength + 1] {};

        strcpy(str, lhs.getData());
        strcat(str, rhs.getData());

        String result(str);

        return result;
    }
}

String operator+(const String& lhs, const char* rhs) {
    lhs.assertData(rhs);

    size_t concatenatedLength = lhs.getLength() + strlen(rhs);

    if(lhs.canBeOptimized(concatenatedLength)) {
        char str[MAX_SMALL_STRING_LENGTH + 1] = {};

        strcpy(str, lhs.getData());
        strcat(str, rhs);

        String result(str);

        return result;

    }else {
        char* str = new char[concatenatedLength + 1] {};

        strcpy(str, lhs.getData());
        strcat(str, rhs);

        String result(str);

        return result;
    }
}

String operator+(const String& lhs, char symbol) {
    String symbolStr(" ");
    symbolStr[0] = symbol;

    return operator+(lhs, symbolStr);
}

String operator+(const String& lhs, size_t number) {
    size_t digitCount = numberOfDigits(number);
    char temp[MAX_SIZE_T_DIGITS + 1] = {};

    for(size_t i = 0; i < digitCount; ++i) {
        temp[i] = getDigit(number, i, digitCount);
    }

    String numStr(temp);

    return operator+(lhs, numStr);
}

String operator+(const String& lhs, unsigned number) {
    size_t convertedNumber = number;

    return operator+(lhs, convertedNumber);
}

std::ostream& operator<<(std::ostream& os, const String& str) {

    return os << str.getData();
}

std::istream& operator>>(std::istream& is, String& str) {
    char buffer[BUFFER_SIZE];
    is.getline(buffer, BUFFER_SIZE);

    str.setMembers(buffer);

    return is;
}

bool operator<(const String& lhs, const String& rhs) {

    return strcmp(lhs.getData(), rhs.getData()) < 0;
}

bool operator<=(const String& lhs, const String& rhs) {

    return strcmp(lhs.getData(), rhs.getData()) <= 0;
}

bool operator>=(const String& lhs, const String& rhs) {

    return strcmp(lhs.getData(), rhs.getData()) >= 0;
}

bool operator>(const String& lhs, const String& rhs) {

    return strcmp(lhs.getData(), rhs.getData()) > 0;
}

bool operator==(const String& lhs, const String& rhs) {

    return strcmp(lhs.getData(), rhs.getData()) == 0;
}

bool operator!=(const String& lhs, const String& rhs) {

    return strcmp(lhs.getData(), rhs.getData()) != 0;
}
