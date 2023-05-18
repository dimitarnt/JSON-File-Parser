#pragma once
#include "String.h"
#include "Vector.hpp"
#include "JsonNodeCollection.h"

class JsonObject : public JsonNode {
private:
    JsonNodeCollection _jsonNodeCollection;
    Vector<String> _correspondingKeys;

public:
    JsonObject() = delete;
    explicit JsonObject(std::ifstream& in);
    ~JsonObject() override = default;

    void print(unsigned nestingLevel) const override;

    JsonNode* clone() const override;
};
