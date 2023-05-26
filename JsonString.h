#pragma once
#include "String.h"
#include "JsonNode.h"

class JsonString : public JsonNode {
private:
    String _value;

public:
    JsonString() = delete;
    explicit JsonString(std::ifstream& in);

    void print(unsigned nestingLevel, bool isInArray) const override;

    JsonNode* clone() const override;
};
