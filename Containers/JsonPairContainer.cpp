#include "JsonPairContainer.h"
#include "JsonNodeFactory.h"

unsigned JsonPairContainer::getSize() const {
    return _jsonPairs.getSize();
}

const JsonPair& JsonPairContainer::operator[](unsigned index) const {
    return _jsonPairs[index];
}

JsonPair& JsonPairContainer::operator[](unsigned index) {
    return _jsonPairs[index];
}

const String& JsonPairContainer::getKey(unsigned index) const {
    return _jsonPairs[index].getKey();
}

JsonKey& JsonPairContainer::accessKey(unsigned index) {
    return _jsonPairs[index].accessKey();
}

void JsonPairContainer::setKey(const String& key, unsigned index) {
    _jsonPairs[index].setKey(key);
}

void JsonPairContainer::setKey(String&& key, unsigned index) {
    _jsonPairs[index].setKey(std::move(key));
}

SharedPtr<JsonNode> JsonPairContainer::getJsonNode(unsigned index) const {
    return _jsonPairs[index].getNode();
}

SharedPtr<JsonNode>& JsonPairContainer::accessJsonNode(unsigned index) {
    return _jsonPairs[index].accessNode();
}

void JsonPairContainer::setJsonNode(const SharedPtr<JsonNode>& node, unsigned index) {
    _jsonPairs[index].setNode(node);
}

void JsonPairContainer::setJsonNode(SharedPtr<JsonNode>&& node, unsigned index) {
    _jsonPairs[index].setNode(std::move(node));
}

void JsonPairContainer::addJsonPair(const JsonKey& key, JsonNodeType type, std::ifstream& in) {
    addJsonPair(key, type, in , getSize());
}

void JsonPairContainer::addJsonPair(JsonKey&& key, JsonNodeType type, std::ifstream& in) {
    addJsonPair(std::move(key), type, in , getSize());
}

void JsonPairContainer::addJsonPair(const JsonKey& key, const SharedPtr<JsonNode>& newJsonNode) {
    addJsonPair(key, newJsonNode, getSize());
}

void JsonPairContainer::addJsonPair(JsonKey&& key, const SharedPtr<JsonNode>& newJsonNode) {
    addJsonPair(std::move(key), newJsonNode, getSize());
}

void JsonPairContainer::addJsonPair(const JsonKey& key, SharedPtr<JsonNode>&& newJsonNode) {
    addJsonPair(key, std::move(newJsonNode), getSize());
}

void JsonPairContainer::addJsonPair(JsonKey&& key, SharedPtr<JsonNode>&& newJsonNode) {
    addJsonPair(std::move(key), std::move(newJsonNode), getSize());
}

void JsonPairContainer::addJsonPair(const JsonKey& key, JsonNodeType type, std::ifstream& in, size_t index) {

    _jsonPairs.pushAt(JsonPair(key, JsonNodeFactory::create(in, type)), index);
}

void JsonPairContainer::addJsonPair(JsonKey&& key, JsonNodeType type, std::ifstream& in, size_t index) {

    _jsonPairs.pushAt(JsonPair(std::move(key), JsonNodeFactory::create(in, type)), index);
}

void JsonPairContainer::addJsonPair(const JsonKey& key, const SharedPtr<JsonNode>& newJsonNode, size_t index) {
    _jsonPairs.pushAt(JsonPair(key, newJsonNode), index);
}

void JsonPairContainer::addJsonPair(JsonKey&& key, const SharedPtr<JsonNode>& newJsonNode, size_t index) {
    _jsonPairs.pushAt(JsonPair(std::move(key), newJsonNode), index);
}

void JsonPairContainer::addJsonPair(const JsonKey& key, SharedPtr<JsonNode>&& newJsonNode, size_t index) {
    _jsonPairs.pushAt(JsonPair(key, std::move(newJsonNode)), index);
}

void JsonPairContainer::addJsonPair(JsonKey&& key, SharedPtr<JsonNode>&& newJsonNode, size_t index) {
    _jsonPairs.pushAt(JsonPair(std::move(key), std::move(newJsonNode)), index);
}

void JsonPairContainer::addJsonPair(const JsonPair& newJsonPair) {
    addJsonPair(newJsonPair, getSize());
}

void JsonPairContainer::addJsonPair(JsonPair&& newJsonPair) {
    addJsonPair(std::move(newJsonPair), getSize());
}

void JsonPairContainer::addJsonPair(const JsonPair& newJsonPair, size_t index) {
    _jsonPairs.pushAt(JsonPair(newJsonPair.getKey(), newJsonPair.getNode()), index);
}

void JsonPairContainer::addJsonPair(JsonPair&& newJsonPair, size_t index) {
    _jsonPairs.pushAt(JsonPair(std::move(newJsonPair.accessKey()), std::move(newJsonPair.accessNode())), index);
}

void JsonPairContainer::addEmptyJsonObjectPair(const JsonKey& key) {
    addEmptyJsonObjectPair(key, getSize());
}

void JsonPairContainer::addEmptyJsonObjectPair(JsonKey&& key) {
    addEmptyJsonObjectPair(std::move(key), getSize());
}

void JsonPairContainer::addEmptyJsonArrayPair(const JsonKey& key) {
    addEmptyJsonArrayPair(key, getSize());
}

void JsonPairContainer::addEmptyJsonArrayPair(JsonKey&& key) {
    addEmptyJsonArrayPair(std::move(key), getSize());
}

void JsonPairContainer::addEmptyJsonObjectPair(const JsonKey& key, size_t index) {
    _jsonPairs.pushAt(JsonPair(key, JsonNodeFactory::createEmptyJsonObject()), index);
}

void JsonPairContainer::addEmptyJsonObjectPair(JsonKey&& key, size_t index) {
    _jsonPairs.pushAt(JsonPair(std::move(key), JsonNodeFactory::createEmptyJsonObject()), index);
}

void JsonPairContainer::addEmptyJsonArrayPair(const JsonKey& key, size_t index) {
    _jsonPairs.pushAt(JsonPair(key, JsonNodeFactory::createEmptyJsonArray()), index);
}

void JsonPairContainer::addEmptyJsonArrayPair(JsonKey&& key, size_t index) {
    _jsonPairs.pushAt(JsonPair(std::move(key), JsonNodeFactory::createEmptyJsonArray()), index);
}

void JsonPairContainer::addJsonStringPair(const JsonKey& key, const String& str) {
    addJsonStringPair(key, str, getSize());
}

void JsonPairContainer::addJsonStringPair(JsonKey&& key, const String& str) {
    addJsonStringPair(std::move(key), str, getSize());
}

void JsonPairContainer::addJsonStringPair(const JsonKey& key, String&& str) {
    addJsonStringPair(key, std::move(str), getSize());
}

void JsonPairContainer::addJsonStringPair(JsonKey&& key, String&& str) {
    addJsonStringPair(std::move(key), std::move(str), getSize());
}

void JsonPairContainer::addJsonStringPair(const JsonKey& key, const String& str, size_t index) {
    _jsonPairs.pushAt(JsonPair(key, JsonNodeFactory::createJsonString(str)), index);
}

void JsonPairContainer::addJsonStringPair(JsonKey&& key, const String& str, size_t index) {
    _jsonPairs.pushAt(JsonPair(std::move(key), JsonNodeFactory::createJsonString(str)), index);
}

void JsonPairContainer::addJsonStringPair(const JsonKey& key, String&& str, size_t index) {
    _jsonPairs.pushAt(JsonPair(key, JsonNodeFactory::createJsonString(std::move(str))), index);
}

void JsonPairContainer::addJsonStringPair(JsonKey&& key, String&& str, size_t index) {
    _jsonPairs.pushAt(JsonPair(std::move(key), JsonNodeFactory::createJsonString(std::move(str))), index);
}

void JsonPairContainer::addJsonKeywordPair(const JsonKey& key, const String& keyword) {
    addJsonKeywordPair(key, keyword, getSize());
}

void JsonPairContainer::addJsonKeywordPair(JsonKey&& key, const String& keyword) {
    addJsonKeywordPair(std::move(key), keyword, getSize());
}

void JsonPairContainer::addJsonKeywordPair(const JsonKey& key, String&& keyword) {
    addJsonKeywordPair(key, std::move(keyword), getSize());
}

void JsonPairContainer::addJsonKeywordPair(JsonKey&& key, String&& keyword) {
    addJsonKeywordPair(std::move(key), std::move(keyword), getSize());
}

void JsonPairContainer::addJsonKeywordPair(const JsonKey& key, const String& keyword, size_t index) {
    _jsonPairs.pushAt(JsonPair(key, JsonNodeFactory::createJsonKeyword(keyword)), index);
}

void JsonPairContainer::addJsonKeywordPair(JsonKey&& key, const String& keyword, size_t index) {
    _jsonPairs.pushAt(JsonPair(std::move(key), JsonNodeFactory::createJsonKeyword(keyword)), index);
}

void JsonPairContainer::addJsonKeywordPair(const JsonKey& key, String&& keyword, size_t index) {
    _jsonPairs.pushAt(JsonPair(key, JsonNodeFactory::createJsonKeyword(std::move(keyword))), index);
}

void JsonPairContainer::addJsonKeywordPair(JsonKey&& key, String&& keyword, size_t index) {
    _jsonPairs.pushAt(JsonPair(std::move(key), JsonNodeFactory::createJsonKeyword(std::move(keyword))), index);
}

void JsonPairContainer::addJsonNumberPair(const JsonKey& key, const String& number) {
    addJsonNumberPair(key, number, getSize());
}

void JsonPairContainer::addJsonNumberPair(JsonKey&& key, const String& number) {
    addJsonNumberPair(std::move(key), number, getSize());
}

void JsonPairContainer::addJsonNumberPair(const JsonKey& key, String&& number) {
    addJsonNumberPair(key, std::move(number), getSize());
}

void JsonPairContainer::addJsonNumberPair(JsonKey&& key, String&& number) {
    addJsonNumberPair(std::move(key), std::move(number), getSize());
}

void JsonPairContainer::addJsonNumberPair(const JsonKey& key, const String& number, size_t index) {
    _jsonPairs.pushAt(JsonPair(key, JsonNodeFactory::createJsonNumber(number)), index);
}

void JsonPairContainer::addJsonNumberPair(JsonKey&& key, const String& number, size_t index) {
    _jsonPairs.pushAt(JsonPair(std::move(key), JsonNodeFactory::createJsonNumber(number)), index);
}

void JsonPairContainer::addJsonNumberPair(const JsonKey& key, String&& number, size_t index) {
    _jsonPairs.pushAt(JsonPair(key, JsonNodeFactory::createJsonNumber(std::move(number))), index);
}

void JsonPairContainer::addJsonNumberPair(JsonKey&& key, String&& number, size_t index) {
    _jsonPairs.pushAt(JsonPair(std::move(key), JsonNodeFactory::createJsonNumber(std::move(number))), index);
}

JsonPair JsonPairContainer::removeJsonPairByIndex(unsigned index) {
    return _jsonPairs.popAt(index);
}
