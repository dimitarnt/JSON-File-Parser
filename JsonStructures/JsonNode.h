#pragma once
#include "String.h"

class JsonArray;

enum class JsonNodeType {
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_STRING,
    JSON_KEYWORD,
    JSON_NUMBER
};

namespace {
    const int NUMBER_OF_INDENTATION_SPACES = 2;
}

//Abstract base class
class JsonNode {
private:
    JsonNodeType _type;

public:
    explicit JsonNode(JsonNodeType type);
    virtual ~JsonNode() = default;
    virtual JsonNode* clone() const = 0;

    JsonNodeType getType() const;

    virtual void print(unsigned nestingLevel) const = 0;
    virtual void save(std::ofstream& out, unsigned nestingLevel) const = 0;

    //Get a json value or a json string from a file
    static String parseValue(std::ifstream& in);

    static void printIndentation(unsigned nestingLevel);
    static void putIndentationInFile(std::ofstream& out, unsigned nestingLevel);

    static unsigned lastNestingLevelInPath(const char* path);
    static String getKeyInPath(const char* path, unsigned nestingLevel);
};
