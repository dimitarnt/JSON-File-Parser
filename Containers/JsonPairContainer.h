#pragma once
#include "JsonKey.h"
#include "JsonPair.h"
#include "Vector.hpp"

//Heterogeneous container for pairs comprised of a key and a json node
class JsonPairContainer {
private:
    Vector<JsonPair> _jsonPairs;

public:
    JsonPairContainer() = default;

    unsigned getSize() const;

    const JsonPair& operator[](unsigned index) const;
    JsonPair& operator[](unsigned index);

    const String& getKey(unsigned index) const;
    JsonKey& accessKey(unsigned index);
    void setKey(const String& key, unsigned index);
    void setKey(String&& key, unsigned index);

    SharedPtr<JsonNode> getJsonNode(unsigned index) const;
    SharedPtr<JsonNode>& accessJsonNode(unsigned index);
    void setJsonNode(const SharedPtr<JsonNode>& node, unsigned index);
    void setJsonNode(SharedPtr<JsonNode>&& node, unsigned index);

    void addJsonPair(const JsonKey& key, JsonNodeType type, std::ifstream& in);
    void addJsonPair(JsonKey&& key, JsonNodeType type, std::ifstream& in);
    void addJsonPair(const JsonKey& key, const SharedPtr<JsonNode>& newJsonNode);
    void addJsonPair(JsonKey&& key, const SharedPtr<JsonNode>& newJsonNode);
    void addJsonPair(const JsonKey& key, SharedPtr<JsonNode>&& newJsonNode);
    void addJsonPair(JsonKey&& key, SharedPtr<JsonNode>&& newJsonNode);

    void addJsonPair(const JsonKey& key, JsonNodeType type, std::ifstream& in, size_t index);
    void addJsonPair(JsonKey&& key, JsonNodeType type, std::ifstream& in, size_t index);
    void addJsonPair(const JsonKey& key, const SharedPtr<JsonNode>& newJsonNode, size_t index);
    void addJsonPair(JsonKey&& key, const SharedPtr<JsonNode>& newJsonNode, size_t index);
    void addJsonPair(const JsonKey& key, SharedPtr<JsonNode>&& newJsonNode, size_t index);
    void addJsonPair(JsonKey&& key, SharedPtr<JsonNode>&& newJsonNode, size_t index);

    void addJsonPair(const JsonPair& newJsonPair);
    void addJsonPair(JsonPair&& newJsonPair);
    void addJsonPair(const JsonPair& newJsonPair, size_t index);
    void addJsonPair(JsonPair&& newJsonPair, size_t index);

    void addEmptyJsonObjectPair(const JsonKey& key);
    void addEmptyJsonObjectPair(JsonKey&& key);
    void addEmptyJsonArrayPair(const JsonKey& key);
    void addEmptyJsonArrayPair(JsonKey&& key);

    void addEmptyJsonObjectPair(const JsonKey& key, size_t index);
    void addEmptyJsonObjectPair(JsonKey&& key, size_t index);
    void addEmptyJsonArrayPair(const JsonKey& key, size_t index);
    void addEmptyJsonArrayPair(JsonKey&& key, size_t index);

    void addJsonStringPair(const JsonKey& key, const String& str);
    void addJsonStringPair(JsonKey&& key, const String& str);
    void addJsonStringPair(const JsonKey& key, String&& str);
    void addJsonStringPair(JsonKey&& key, String&& str);

    void addJsonStringPair(const JsonKey& key, const String& str, size_t index);
    void addJsonStringPair(JsonKey&& key, const String& str, size_t index);
    void addJsonStringPair(const JsonKey& key, String&& str, size_t index);
    void addJsonStringPair(JsonKey&& key, String&& str, size_t index);

    void addJsonKeywordPair(const JsonKey& key, const String& keyword);
    void addJsonKeywordPair(JsonKey&& key, const String& keyword);
    void addJsonKeywordPair(const JsonKey& key, String&& keyword);
    void addJsonKeywordPair(JsonKey&& key, String&& keyword);

    void addJsonKeywordPair(const JsonKey& key, const String& keyword, size_t index);
    void addJsonKeywordPair(JsonKey&& key, const String& keyword, size_t index);
    void addJsonKeywordPair(const JsonKey& key, String&& keyword, size_t index);
    void addJsonKeywordPair(JsonKey&& key, String&& keyword, size_t index);

    void addJsonNumberPair(const JsonKey& key, const String& number);
    void addJsonNumberPair(JsonKey&& key, const String& number);
    void addJsonNumberPair(const JsonKey& key, String&& number);
    void addJsonNumberPair(JsonKey&& key, String&& number);

    void addJsonNumberPair(const JsonKey& key, const String& number, size_t index);
    void addJsonNumberPair(JsonKey&& key, const String& number, size_t index);
    void addJsonNumberPair(const JsonKey& key, String&& number, size_t index);
    void addJsonNumberPair(JsonKey&& key, String&& number, size_t index);

    JsonPair removeJsonPairByIndex(unsigned index);
};

