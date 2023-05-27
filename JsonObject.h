#pragma once
#include "Vector.hpp"
#include "JsonArray.h"
#include "JsonNodeCollection.h"

class JsonObject : public JsonNode {
private:
    JsonNodeCollection _jsonNodeCollection;
    Vector<String> _correspondingKeys;

    long long findKeyIndex(const String& key) const;

public:
    JsonObject() = delete;
    explicit JsonObject(std::ifstream& in);
    JsonNode* clone() const override;

    void print(unsigned nestingLevel, bool isInArray) const override;

    void search(JsonArray& searchResults, const String& keyStr) const override;

    void set(const char* path, const char* newStr, unsigned nestingLevel) override;
    void remove(const char* path, unsigned nestingLevel) override;
};
