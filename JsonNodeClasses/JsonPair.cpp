#include "JsonPair.h"

JsonPair::JsonPair(const JsonKey& key, const SharedPtr<JsonNode>& node) : _key(key), _node(node) {}

JsonPair::JsonPair(const JsonKey& key, SharedPtr<JsonNode>&& node) : _key(key), _node(node) {}

JsonPair::JsonPair(JsonKey&& key, const SharedPtr<JsonNode>& node) : _key(std::move(key)), _node(node) {}

JsonPair::JsonPair(JsonKey&& key, SharedPtr<JsonNode>&& node) : _key(std::move(key)), _node(std::move(node)) {}

const String& JsonPair::getKey() const {
    return _key.getKeyString();
}

JsonKey& JsonPair::accessKey() {
    return _key;
}

const SharedPtr<JsonNode>& JsonPair::getNode() const {
    return _node;
}

SharedPtr<JsonNode>& JsonPair::accessNode() {
    return _node;
}

void JsonPair::setKey(const JsonKey& key) {
    _key = key;
}

void JsonPair::setKey(JsonKey&& key) {
    _key = std::move(key);
}

void JsonPair::setNode(const SharedPtr<JsonNode>& node) {
    _node = node;
}

void JsonPair::setNode(SharedPtr<JsonNode>&& node) {
    _node = std::move(node);
}
