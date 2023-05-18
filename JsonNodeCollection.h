#pragma once
#include "Vector.hpp"
#include "JsonNode.h"

class JsonNodeCollection {
private:
    Vector<JsonNode*> _jsonNodes;

    void copyFrom(const JsonNodeCollection& other);
    void free();

public:
    JsonNodeCollection() = default;
    JsonNodeCollection(const JsonNodeCollection& other);
    JsonNodeCollection(JsonNodeCollection&& other) noexcept;
    JsonNodeCollection& operator=(const JsonNodeCollection& other);
    JsonNodeCollection& operator=(JsonNodeCollection&& other) noexcept;
    ~JsonNodeCollection();

    unsigned getSize() const;
    const JsonNode* operator[](unsigned index) const;
    JsonNodeType getTypeByIndex(unsigned index) const;

    void addJsonNode(JsonNodeType type, std::ifstream& in);
};

JsonNode* jsonNodeFactory(JsonNodeType type, std::ifstream& in);
