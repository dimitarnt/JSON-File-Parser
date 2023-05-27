#pragma once
#include "Vector.hpp"
#include "JsonNodeCollection.h"

class JsonArray : public JsonNode {
private:
    JsonNodeCollection _jsonNodeCollection;

    static void assertNaturalNumberFromStr(const String& index, unsigned nestingLevel);
    void assertIndex(size_t index, unsigned nestingLevel) const;

public:
    JsonArray();
    explicit JsonArray(std::ifstream& in);
    JsonNode* clone() const override;

    void add(const SharedPtr<JsonNode>& newJsonNode);
    void add(SharedPtr<JsonNode>&& newJsonNode);

    void print(unsigned nestingLevel, bool isInArray) const override;

    void search(JsonArray& searchResults, const String& keyStr) const override;

    void set(const char* path, const char* newStr, unsigned nestingLevel) override;
    void remove(const char* path, unsigned nestingLevel) override;
};
