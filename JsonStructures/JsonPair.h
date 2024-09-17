#pragma once
#include "JsonKey.h"
#include "JsonNode.h"
#include "SharedPtr.hpp"

class JsonPair {
private:
    JsonKey _key;
    SharedPtr<JsonNode> _node;

public:
    JsonPair() = default;
    JsonPair(const JsonKey& key, const SharedPtr<JsonNode>& node);
    JsonPair(const JsonKey& key, SharedPtr<JsonNode>&& node);
    JsonPair(JsonKey&& key, const SharedPtr<JsonNode>& node);
    JsonPair(JsonKey&& key, SharedPtr<JsonNode>&& node);

    const String& getKey() const;
    JsonKey& accessKey();

    const SharedPtr<JsonNode>& getNode() const;
    SharedPtr<JsonNode>& accessNode();

    void setKey(const JsonKey& key);
    void setKey(JsonKey&& key);

    void setNode(const SharedPtr<JsonNode>& node);
    void setNode(SharedPtr<JsonNode>&& node);
};
