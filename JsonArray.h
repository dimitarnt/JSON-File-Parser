#pragma once
#include "Vector.hpp"
#include "JsonNodeCollection.h"

class JsonArray : public JsonNode {
private:
    JsonNodeCollection _jsonNodeCollection;

public:
    JsonArray() = delete;
    explicit JsonArray(std::ifstream& in);
    ~JsonArray() override = default;

    void print(unsigned nestingLevel) const override;

    JsonNode* clone() const override;
};
