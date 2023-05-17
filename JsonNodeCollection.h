#pragma once
#include "Vector.hpp"
#include "JsonNode.h"

class JsonNodeCollection {
private:
    Vector<JsonNode*> _jsonNodes;

    void copyFrom(const JsonNodeCollection& other);
    void free();

    void addJsonNode(JsonNode* newJsonNode);

public:
    JsonNodeCollection() = default;
    JsonNodeCollection(const JsonNodeCollection& other);
    JsonNodeCollection(JsonNodeCollection&& other) noexcept;
    JsonNodeCollection& operator=(const JsonNodeCollection& other);
    JsonNodeCollection& operator=(JsonNodeCollection&& other) noexcept;
    ~JsonNodeCollection();

    unsigned getSize() const;

    void addJsonObject(std::ifstream& in, unsigned nestingLevel);
    void addJsonArray(std::ifstream& in, unsigned nestingLevel);
    void addJsonString(std::ifstream& in);
    void addJsonValue(std::ifstream& in);

    const JsonNode* operator[](unsigned index) const;
};
