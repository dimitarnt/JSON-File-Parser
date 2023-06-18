#pragma once
#include <iostream>
#include <fstream>

long long pow(int num, size_t n);

size_t numberOfDigits(size_t num);

char getDigit(size_t num, size_t index, size_t digitCount);

size_t stringToSizeT(const char* str);

unsigned getCharCount(const char* str, size_t strLength, char symbol);

long long getPositionOfChar(const char* str, size_t strLength, char token, unsigned timesMet);
