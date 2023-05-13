#pragma once
#include "JsonValidator.h"
#include "JsonNode.h"

class JsonParser {
private:
    String _fileName;
    JsonNode* _startingNode;

    void assertJsonFileName(const char* fileName) const;

public:
    explicit JsonParser(const char* fileName);

    void setFile(const char* fileName);

    void print() const;

};
