#pragma once
#include "Vector.hpp"
#include "JsonNodeCollection.h"

class JsonArray : public JsonNode {
private:
    JsonNodeCollection _jsonNodeCollection;

public:
    JsonArray();
    explicit JsonArray(std::ifstream& in);
    JsonNode* clone() const override;

    void add(const SharedPtr<JsonNode>& newJsonNode);

    void print(unsigned nestingLevel, bool isInArray) const override;

    void search(JsonArray& searchResults, const String& keyStr) const override;
};
