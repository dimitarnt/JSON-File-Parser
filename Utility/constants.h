#pragma once
#include <iostream>

const int NUMBER_OF_BYTES_IN_CHAR_PTR = sizeof(char*);
const int NUMBER_OF_BYTES_IN_SIZE_T = sizeof(size_t);

const int NUMBER_OF_DIGITS = 10;
const int NUMBER_OF_JSON_KEYWORDS = 3;

const char* const JSON_FILE_EXTENSION = ".json";

const int MAX_SIZE_T_DIGITS = 20;
const int MAX_UNSIGNED_INT_DIGITS = 10;

const int BUFFER_SIZE = 1024;

const unsigned char BITS[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
