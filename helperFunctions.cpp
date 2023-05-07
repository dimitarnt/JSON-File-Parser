#include "helperFunctions.h"

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

unsigned getFileSize(std::ifstream& file) {
    unsigned currentPos = file.tellg();

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(currentPos);

    return fileSize;
}

unsigned getCharCountFromFile(std::ifstream& in, char symbol) {
    unsigned currentPosition = in.tellg();
    in.seekg(0, std::ios::beg);

    if (!in.is_open()) {
        return 0;
    }

    unsigned count = 0;

    while (true) {
        char current = (char)in.get();

        if (in.eof()) {
            break;
        }

        if (current == symbol) {
            count++;
        }
    }

    in.clear();
    in.seekg(currentPosition);

    return count;
}

unsigned getCharCountFromFile(std::ifstream& in, char symbol, unsigned untilPosition) {
    if(untilPosition > getFileSize(in)) {
        throw std::out_of_range("Position is out of range");
    }

    unsigned currentPosition = in.tellg();
    in.seekg(0, std::ios::beg);

    if (!in.is_open()) {
        return 0;
    }

    unsigned count = 0;

    while(true) {
        char current = (char)in.get();

        if (current == symbol) {
            count++;
        }

        if(in.tellg() == untilPosition) {
            break;
        }
    }

    in.clear();
    in.seekg(currentPosition);

    return count;
}

unsigned getLinesCount(std::ifstream& in) {

    return getCharCountFromFile(in, '\n') + 1;
}

unsigned getLinesCount(std::ifstream& in, unsigned untilPosition) {

    return getCharCountFromFile(in, '\n', untilPosition) + 1;
}
