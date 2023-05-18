#pragma once
#include "String.h"

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

    virtual void print(unsigned nestingLevel) const = 0;
    static void printIndentation(unsigned nestingLevel);
};
