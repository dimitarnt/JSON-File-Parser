#pragma once
#include "Vector.hpp"
#include "SharedPtr.hpp"
#include "JsonNode.h"

//Heterogeneous container for json nodes
class JsonNodeContainer {
private:
    Vector<SharedPtr<JsonNode>> _jsonNodes;

public:
    JsonNodeContainer() = default;

    unsigned getSize() const;
    const SharedPtr<JsonNode>& operator[](unsigned index) const;
    SharedPtr<JsonNode>& operator[](unsigned index);

    void addJsonNode(JsonNodeType type, std::ifstream& in);
    void addJsonNode(const SharedPtr<JsonNode>& newJsonNode);
    void addJsonNode(SharedPtr<JsonNode>&& newJsonNode);

    void addJsonNode(JsonNodeType type, std::ifstream& in, size_t index);
    void addJsonNode(const SharedPtr<JsonNode>& newJsonNode, size_t index);
    void addJsonNode(SharedPtr<JsonNode>&& newJsonNode, size_t index);

    void addEmptyJsonObject();
    void addEmptyJsonArray();

    void addEmptyJsonObject(size_t index);
    void addEmptyJsonArray(size_t index);

    void addJsonString(const String& str);
    void addJsonString(String&& str);

    void addJsonString(const String& str, size_t index);
    void addJsonString(String&& str, size_t index);

    void addJsonKeyword(const String& keyword);
    void addJsonKeyword(String&& keyword);

    void addJsonKeyword(const String& keyword, size_t index);
    void addJsonKeyword(String&& keyword, size_t index);

    void addJsonNumber(const String& number);
    void addJsonNumber(String&& number);

    void addJsonNumber(const String& number, size_t index);
    void addJsonNumber(String&& number, size_t index);

    SharedPtr<JsonNode> removeJsonNodeByIndex(unsigned index);
};
