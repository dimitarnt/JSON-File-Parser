#pragma once
#include "String.h"
#include "JsonNode.h"

class JsonValue : public JsonNode {
private:
    String _value;

public:
    JsonValue() = delete;
    explicit JsonValue(std::ifstream& in);

    void print(unsigned nestingLevel, bool isInArray) const override;
    void save(std::ofstream& out, unsigned nestingLevel, bool isInArray) const override;

    JsonNode* clone() const override;
};
