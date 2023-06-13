#pragma once
#include "String.h"
#include "Pair.hpp"
#include "JsonNodeContainer.h"

class JsonPairContainer {
private:
    Vector<Pair<String, SharedPtr<JsonNode>>> _jsonPairs;

public:
    JsonPairContainer() = default;

    unsigned getSize() const;
    String getKey(unsigned index) const;
    String& accessKey(unsigned index);
    SharedPtr<JsonNode> getJsonNode(unsigned index) const;
    SharedPtr<JsonNode>& accessJsonNode(unsigned index);

    void addJsonPair(const String& key, JsonNodeType type, std::ifstream& in);
    void addJsonPair(String&& key, JsonNodeType type, std::ifstream& in);
    void addJsonPair(const char* key, JsonNodeType type, std::ifstream& in);
    void addJsonPair(const String& key, const SharedPtr<JsonNode>& newJsonNode);
    void addJsonPair(String&& key, const SharedPtr<JsonNode>& newJsonNode);
    void addJsonPair(const char* key, const SharedPtr<JsonNode>& newJsonNode);
    void addJsonPair(const String& key, SharedPtr<JsonNode>&& newJsonNode);
    void addJsonPair(String&& key, SharedPtr<JsonNode>&& newJsonNode);
    void addJsonPair(const char* key, SharedPtr<JsonNode>&& newJsonNode);

    void addJsonPair(const String& key, JsonNodeType type, std::ifstream& in, size_t index);
    void addJsonPair(String&& key, JsonNodeType type, std::ifstream& in, size_t index);
    void addJsonPair(const char* key, JsonNodeType type, std::ifstream& in, size_t index);
    void addJsonPair(const String& key, const SharedPtr<JsonNode>& newJsonNode, size_t index);
    void addJsonPair(String&& key, const SharedPtr<JsonNode>& newJsonNode, size_t index);
    void addJsonPair(const char* key, const SharedPtr<JsonNode>& newJsonNode, size_t index);
    void addJsonPair(const String& key, SharedPtr<JsonNode>&& newJsonNode, size_t index);
    void addJsonPair(String&& key, SharedPtr<JsonNode>&& newJsonNode, size_t index);
    void addJsonPair(const char* key, SharedPtr<JsonNode>&& newJsonNode, size_t index);

    void addJsonStringPair(const String& key, const String& value);
    void addJsonStringPair(String&& key, const String& value);
    void addJsonStringPair(const char* key, const String& value);
    void addJsonStringPair(const String& key, String&& value);
    void addJsonStringPair(String&& key, String&& value);
    void addJsonStringPair(const char* key, String&& value);
    void addJsonStringPair(const String& key, const char* value);
    void addJsonStringPair(String&& key, const char* value);
    void addJsonStringPair(const char* key, const char* value);

    void addJsonStringPair(const String& key, const String& value, size_t index);
    void addJsonStringPair(String&& key, const String& value, size_t index);
    void addJsonStringPair(const char* key, const String& value, size_t index);
    void addJsonStringPair(const String& key, String&& value, size_t index);
    void addJsonStringPair(String&& key, String&& value, size_t index);
    void addJsonStringPair(const char* key, String&& value, size_t index);
    void addJsonStringPair(const String& key, const char* value, size_t index);
    void addJsonStringPair(String&& key, const char* value, size_t index);
    void addJsonStringPair(const char* key, const char* value, size_t index);

    Pair<String, SharedPtr<JsonNode>> removeJsonPairByIndex(unsigned index);
};

