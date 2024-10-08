#pragma once
#include <iostream>
#include <fstream>
#include "String.h"

long long pow(int num, size_t n);

size_t numberOfDigits(size_t num);

char getDigit(size_t num, size_t index, size_t digitCount);

bool isDigit(char symbol);
