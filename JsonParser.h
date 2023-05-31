#pragma once
#include "JsonValidator.h"
#include "JsonNodeCollection.h"

class JsonParser {
private:
    String _fileName;
    JsonNodeCollection _startingNode;

    static void assertJsonFileName(const char* fileName);
    static void assertString(const char* str);

    void create(const char* path, bool isAddressingStartingNode, bool createInArray, const char* newKey, const char* newStr);

public:
    JsonParser() = delete;
    explicit JsonParser(const char* fileName);

    void setFile(const char* fileName);

    void print() const;
    void search(const char* key) const;

    void set(const char* path, const char* newStr);
    void createInArray(const char* path, const char* newStr);
    void createInObject(const char* newKey, const char* newStr);
    void createInObject(const char* path, const char* newKey, const char* newStr);
    void remove(const char* path);
};
