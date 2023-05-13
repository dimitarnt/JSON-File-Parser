#pragma once
#include "String.h"
#include "JsonNode.h"

class JsonValue : public JsonNode {
private:
    String _value;
    bool _isString;

public:
    JsonValue() = delete;
    explicit JsonValue(std::ifstream& in, bool isString);

    void print() const override;

    JsonNode* clone() const override;

};
