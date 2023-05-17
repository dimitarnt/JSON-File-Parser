#pragma once
#include "JsonValidator.h"
#include "JsonNodeCollection.h"

class JsonParser {
private:
    String _fileName;
    JsonNodeCollection _startingNode;

    static void assertJsonFileName(const char* fileName);

public:
    JsonParser() = delete;
    explicit JsonParser(const char* fileName);

    void setFile(const char* fileName);

    void print() const;
};
