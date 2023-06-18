#include "fileFunctions.h"

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

void getCharactersUntilDelimiter(std::ifstream& in, String& str, unsigned bufferSize, char delimiter) {
    char currentChar = 0;

    for(unsigned i = 1; i < bufferSize; ++i) {
        currentChar = (char)in.peek();

        if(currentChar == delimiter) {
            return;
        }

        in.get();

        if(currentChar != '\n') {
            str += currentChar;
        }

        if(in.eof()) {
            return;
        }
    }
}

void getCharactersUntilDelimiter(std::ifstream& in, String& str, unsigned bufferSize,
                                 char delimiter1, char delimiter2, char delimiter3, char delimiter4, char delimiter5) {
    char currentChar = 0;

    for(unsigned i = 1; i < bufferSize; ++i) {
        currentChar = (char)in.peek();

        if(currentChar == delimiter1 || currentChar == delimiter2 || currentChar == delimiter3
            || currentChar == delimiter4 || currentChar == delimiter5) {
            return;
        }

        in.get();

        if(currentChar != '\n') {
            str += currentChar;
        }

        if(in.eof()) {
            return;
        }
    }
}
