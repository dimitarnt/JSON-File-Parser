#include "helperFunctions.h"
#include <cstring>

long long pow(int num, size_t n) {
    long long result = 1;

    for (size_t i = 0; i < n; i++) {
        result *= num;
    }

    return result;
}

size_t numberOfDigits(size_t num) {
    if(num / 10 == 0) {
        return 1;
    }

    return 1 + numberOfDigits(num / 10);
}

char getDigit(size_t num, size_t index, size_t digitCount) {
    size_t digit = num / pow(10, digitCount - index - 1) % 10;

    return '0' + digit;
}

bool isDigit(char symbol) {

    return symbol >= '0' && symbol <= '9';
}
