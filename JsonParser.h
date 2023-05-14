#pragma once
#include "JsonValidator.h"
#include "JsonNode.h"

class JsonParser {
private:
    String _fileName;
    JsonNode* _startingNode;

    static void assertJsonFileName(const char* fileName);

    void copyFrom(const JsonParser& other);
    void moveFrom(JsonParser&& other);
    void free();

public:
    JsonParser() = delete;
    explicit JsonParser(const char* fileName);

    JsonParser(const JsonParser& other);
    JsonParser(JsonParser&& other) noexcept;
    JsonParser& operator=(const JsonParser& other);
    JsonParser& operator=(JsonParser&& other) noexcept;
    ~JsonParser();

    void setFile(const char* fileName);

    void print() const;

};
