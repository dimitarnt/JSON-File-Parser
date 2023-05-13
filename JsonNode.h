#pragma once
#include "String.h"

namespace {
    const int NUMBER_OF_INDENTATION_SPACES = 2;
}

class JsonNode {
public:
    JsonNode() = default;
    virtual ~JsonNode() = default;

    String parseValue(std::ifstream& in) const;

    virtual void print() const = 0;
    static void printIndentation(unsigned nestingLevel);

    virtual JsonNode* clone() const = 0;
};
