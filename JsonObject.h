#pragma once
#include "String.h"
#include "Vector.hpp"
#include "JsonNodeCollection.h"

class JsonObject : public JsonNode {
private:
    JsonNodeCollection _jsonNodeCollection;
    Vector<String> _correspondingKeys;
    unsigned _nestingLevel;

public:
    JsonObject() = delete;
    JsonObject(std::ifstream& in, unsigned nestingLevel);
    ~JsonObject() override = default;

    void print() const override;

    JsonNode* clone() const override;
};
