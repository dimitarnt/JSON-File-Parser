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

    virtual void print(unsigned nestingLevel, bool isInArray) const = 0;
    virtual void save(std::ofstream& out, unsigned nestingLevel, bool isInArray) const = 0;

    static String parseValue(std::ifstream& in);
    static void printIndentation(unsigned nestingLevel);
    static void putIndentationInFile(std::ofstream& out, unsigned nestingLevel);

    static unsigned lastNestingLevelInPath(const char* path);
    static String getKeyInPath(const char* path, unsigned nestingLevel);
};
