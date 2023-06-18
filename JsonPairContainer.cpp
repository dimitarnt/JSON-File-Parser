#include "JsonPairContainer.h"
#include "JsonObjectFactory.h"
#include "JsonArrayFactory.h"
#include "JsonValueFactory.h"
#include "JsonStringFactory.h"

unsigned JsonPairContainer::getSize() const {
    return _jsonPairs.getSize();
}

String JsonPairContainer::getKey(unsigned index) const {
    return _jsonPairs[index].getFirst();
}

String& JsonPairContainer::accessKey(unsigned index) {
    return _jsonPairs[index].accessFirst();
}

SharedPtr<JsonNode> JsonPairContainer::getJsonNode(unsigned index) const {
    return _jsonPairs[index].getSecond();
}

SharedPtr<JsonNode>& JsonPairContainer::accessJsonNode(unsigned index) {
    return _jsonPairs[index].accessSecond();
}

void JsonPairContainer::addJsonPair(const String& key, JsonNodeType type, std::ifstream& in) {
    addJsonPair(key, type, in , getSize());
}

void JsonPairContainer::addJsonPair(String&& key, JsonNodeType type, std::ifstream& in) {
    addJsonPair(std::move(key), type, in , getSize());
}

void JsonPairContainer::addJsonPair(const String& key, const SharedPtr<JsonNode>& newJsonNode) {
    addJsonPair(key, newJsonNode, getSize());
}

void JsonPairContainer::addJsonPair(String&& key, const SharedPtr<JsonNode>& newJsonNode) {
    addJsonPair(std::move(key), newJsonNode, getSize());
}

void JsonPairContainer::addJsonPair(const String& key, SharedPtr<JsonNode>&& newJsonNode) {
    addJsonPair(key, std::move(newJsonNode), getSize());
}

void JsonPairContainer::addJsonPair(String&& key, SharedPtr<JsonNode>&& newJsonNode) {
    addJsonPair(std::move(key), std::move(newJsonNode), getSize());
}

void JsonPairContainer::addJsonPair(const String& key, JsonNodeType type, std::ifstream& in, size_t index) {
    JsonObjectFactory objectFactory;
    JsonArrayFactory arrayFactory;
    JsonValueFactory valueFactory;
    JsonStringFactory stringFactory;

    switch(type) {

        case JsonNodeType::JSON_OBJECT:
            _jsonPairs.pushAt(Pair<String, SharedPtr<JsonNode>>(key, objectFactory.create(in)), index);
            break;

        case JsonNodeType::JSON_ARRAY:
            _jsonPairs.pushAt(Pair<String, SharedPtr<JsonNode>>(key, arrayFactory.create(in)), index);
            break;

        case JsonNodeType::JSON_VALUE:
            _jsonPairs.pushAt(Pair<String, SharedPtr<JsonNode>>(key, valueFactory.create(in)), index);
            break;

        case JsonNodeType::JSON_STRING:
            _jsonPairs.pushAt(Pair<String, SharedPtr<JsonNode>>(key, stringFactory.create(in)), index);
            break;
    }
}

void JsonPairContainer::addJsonPair(String&& key, JsonNodeType type, std::ifstream& in, size_t index) {
    JsonObjectFactory objectFactory;
    JsonArrayFactory arrayFactory;
    JsonValueFactory valueFactory;
    JsonStringFactory stringFactory;

    switch(type) {

        case JsonNodeType::JSON_OBJECT:
            _jsonPairs.pushAt(Pair<String, SharedPtr<JsonNode>>(std::move(key), objectFactory.create(in)), index);
            break;

        case JsonNodeType::JSON_ARRAY:
            _jsonPairs.pushAt(Pair<String, SharedPtr<JsonNode>>(std::move(key), arrayFactory.create(in)), index);
            break;

        case JsonNodeType::JSON_VALUE:
            _jsonPairs.pushAt(Pair<String, SharedPtr<JsonNode>>(std::move(key), valueFactory.create(in)), index);
            break;

        case JsonNodeType::JSON_STRING:
            _jsonPairs.pushAt(Pair<String, SharedPtr<JsonNode>>(std::move(key), stringFactory.create(in)), index);
            break;
    }
}

void JsonPairContainer::addJsonPair(const String& key, const SharedPtr<JsonNode>& newJsonNode, size_t index) {
    _jsonPairs.pushAt(Pair<String, SharedPtr<JsonNode>>(key, newJsonNode), index);
}

void JsonPairContainer::addJsonPair(String&& key, const SharedPtr<JsonNode>& newJsonNode, size_t index) {
    _jsonPairs.pushAt(Pair<String, SharedPtr<JsonNode>>(std::move(key), newJsonNode), index);
}

void JsonPairContainer::addJsonPair(const String& key, SharedPtr<JsonNode>&& newJsonNode, size_t index) {
    _jsonPairs.pushAt(Pair<String, SharedPtr<JsonNode>>(key, std::move(newJsonNode)), index);
}

void JsonPairContainer::addJsonPair(String&& key, SharedPtr<JsonNode>&& newJsonNode, size_t index) {
    _jsonPairs.pushAt(Pair<String, SharedPtr<JsonNode>>(std::move(key), std::move(newJsonNode)), index);
}

void JsonPairContainer::addJsonStringPair(const String& key, const String& value) {
    addJsonStringPair(key, value, getSize());
}

void JsonPairContainer::addJsonStringPair(String&& key, const String& value) {
    addJsonStringPair(std::move(key), value, getSize());
}

void JsonPairContainer::addJsonStringPair(const String& key, String&& value) {
    addJsonStringPair(key, std::move(value), getSize());
}

void JsonPairContainer::addJsonStringPair(String&& key, String&& value) {
    addJsonStringPair(std::move(key), std::move(value), getSize());
}

void JsonPairContainer::addJsonStringPair(const String& key, const char* value) {
    addJsonStringPair(key, value, getSize());
}

void JsonPairContainer::addJsonStringPair(String&& key, const char* value) {
    addJsonStringPair(std::move(key), value, getSize());
}

void JsonPairContainer::addJsonStringPair(const String& key, const String& value, size_t index) {
    _jsonPairs.pushAt(Pair<String, SharedPtr<JsonNode>>(key, JsonStringFactory::create(value)), index);
}

void JsonPairContainer::addJsonStringPair(String&& key, const String& value, size_t index) {
    _jsonPairs.pushAt(Pair<String, SharedPtr<JsonNode>>(std::move(key), JsonStringFactory::create(value)), index);
}

void JsonPairContainer::addJsonStringPair(const String& key, String&& value, size_t index) {
    _jsonPairs.pushAt(Pair<String, SharedPtr<JsonNode>>(key, JsonStringFactory::create(std::move(value))), index);
}

void JsonPairContainer::addJsonStringPair(String&& key, String&& value, size_t index) {
    _jsonPairs.pushAt(Pair<String, SharedPtr<JsonNode>>(std::move(key), JsonStringFactory::create(std::move(value))), index);
}

void JsonPairContainer::addJsonStringPair(const String& key, const char* value, size_t index) {
    _jsonPairs.pushAt(Pair<String, SharedPtr<JsonNode>>(key, JsonStringFactory::create(value)), index);
}

void JsonPairContainer::addJsonStringPair(String&& key, const char* value, size_t index) {
    _jsonPairs.pushAt(Pair<String, SharedPtr<JsonNode>>(std::move(key), JsonStringFactory::create(value)), index);
}

Pair<String, SharedPtr<JsonNode>> JsonPairContainer::removeJsonPairByIndex(unsigned index) {
    return _jsonPairs.popAt(index);
}
