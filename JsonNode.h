#pragma once
#include "String.h"

class JsonArray;

enum class JsonNodeType {
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_STRING,
    JSON_VALUE
};

namespace {
    const int NUMBER_OF_INDENTATION_SPACES = 2;
}

class JsonNode {
private:
    JsonNodeType _type;

public:
    explicit JsonNode(JsonNodeType type);
    virtual ~JsonNode() = default;
    virtual JsonNode* clone() const = 0;

    JsonNodeType getType() const;

    static String parseValue(std::ifstream& in);

    virtual void print(unsigned nestingLevel, bool isInArray) const = 0;
    static void printIndentation(unsigned nestingLevel);

    virtual void search(JsonArray& searchResults, const String& keyStr) const;

    static unsigned lastNestingLevelInPath(const char* path);
    static String getKeyInPath(const char* path, unsigned nestingLevel);

    virtual void set(const char* path, const char* newStr, unsigned nestingLevel);
    virtual void create(const char* path, bool isAddressingStartingNode, bool createInArray,
                        const char* newKey, const char* newStr, unsigned nestingLevel);
    virtual void remove(const char* path, unsigned nestingLevel);
};
