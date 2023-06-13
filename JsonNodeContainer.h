#pragma once
#include "Vector.hpp"
#include "SharedPtr.hpp"
#include "JsonNode.h"

class JsonNodeContainer {
private:
    Vector<SharedPtr<JsonNode>> _jsonNodes;

public:
    JsonNodeContainer() = default;

    unsigned getSize() const;
    SharedPtr<JsonNode> operator[](unsigned index) const;
    SharedPtr<JsonNode>& operator[](unsigned index);

    void addJsonNode(JsonNodeType type, std::ifstream& in);
    void addJsonNode(const SharedPtr<JsonNode>& newJsonNode);
    void addJsonNode(SharedPtr<JsonNode>&& newJsonNode);

    void addJsonNode(JsonNodeType type, std::ifstream& in, size_t index);
    void addJsonNode(const SharedPtr<JsonNode>& newJsonNode, size_t index);
    void addJsonNode(SharedPtr<JsonNode>&& newJsonNode, size_t index);

    void addJsonString(const String& value);
    void addJsonString(String&& value);
    void addJsonString(const char* value);

    void addJsonString(const String& value, size_t index);
    void addJsonString(String&& value, size_t index);
    void addJsonString(const char* value, size_t index);

    SharedPtr<JsonNode> removeJsonNodeByIndex(unsigned index);
};
