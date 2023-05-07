#pragma once
#include <iostream>
#include <fstream>

long long pow(int num, size_t n);

size_t numberOfDigits(size_t num);

char getDigit(size_t num, size_t index, size_t digitCount);

unsigned getFileSize(std::ifstream& file);

unsigned getCharCountFromFile(std::ifstream& in, char symbol);

unsigned getCharCountFromFile(std::ifstream& in, char symbol, unsigned untilPosition);

unsigned getLinesCount(std::ifstream& in);

unsigned getLinesCount(std::ifstream& in, unsigned untilPosition);
