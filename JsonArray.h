#pragma once
#include "Vector.hpp"
#include "JsonNodeCollection.h"

class JsonArray : public JsonNode {
private:
    JsonNodeCollection _jsonNodeCollection;
    unsigned _nestingLevel;

public:
    JsonArray() = delete;
    JsonArray(std::ifstream& in, unsigned nestingLevel);
    ~JsonArray() override = default;

    void print() const override;

    JsonNode* clone() const override;
};
