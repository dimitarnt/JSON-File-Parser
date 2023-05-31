#pragma once
#include "Vector.hpp"
#include "SharedPtr.hpp"
#include "JsonNode.h"

class JsonNodeCollection {
private:
    Vector<SharedPtr<JsonNode>> _jsonNodes;

public:
    JsonNodeCollection() = default;

    unsigned getSize() const;
    SharedPtr<JsonNode> operator[](unsigned index) const;
    SharedPtr<JsonNode>& operator[](unsigned index);
    JsonNodeType getTypeByIndex(unsigned index) const;

    void addJsonNode(JsonNodeType type, std::ifstream& in);
    void addJsonNode(const SharedPtr<JsonNode>& newJsonNode);
    void addJsonNode(SharedPtr<JsonNode>&& newJsonNode);

    void addJsonNode(JsonNodeType type, std::ifstream& in, size_t index);
    void addJsonNode(const SharedPtr<JsonNode>& newJsonNode, size_t index);
    void addJsonNode(SharedPtr<JsonNode>&& newJsonNode, size_t index);

    void removeJsonNodeByIndex(unsigned index);
};

SharedPtr<JsonNode> jsonNodeFactory(JsonNodeType type, std::ifstream& in);
