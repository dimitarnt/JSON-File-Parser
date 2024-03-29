#pragma once
#include <fstream>
#include "String.h"

unsigned getFileSize(std::ifstream& file);

unsigned getCharCountFromFile(std::ifstream& in, char symbol);
unsigned getCharCountFromFile(std::ifstream& in, char symbol, unsigned untilPosition);

unsigned getLinesCount(std::ifstream& in);
unsigned getLinesCount(std::ifstream& in, unsigned untilPosition);

void getCharactersUntilDelimiter(std::ifstream& in, String& str, unsigned bufferSize, char delimiter);
void getCharactersUntilDelimiter(std::ifstream& in, String& str, unsigned bufferSize,
                                 char delimiter1, char delimiter2, char delimiter3, char delimiter4, char delimiter5);
