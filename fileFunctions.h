#pragma once
#include <fstream>
#include "String.h"

unsigned getFileSize(std::ifstream& file);

unsigned getCharCountFromFile(std::ifstream& in, char symbol);

unsigned getCharCountFromFile(std::ifstream& in, char symbol, unsigned untilPosition);

unsigned getLinesCount(std::ifstream& in);

unsigned getLinesCount(std::ifstream& in, unsigned untilPosition);

void getLineForString(std::ifstream& in, String& str, unsigned bufferSize, char delimiter);
