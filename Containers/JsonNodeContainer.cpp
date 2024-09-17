#include "JsonNodeContainer.h"
#include "JsonNodeFactory.h"

unsigned JsonNodeContainer::getSize() const {
    return _jsonNodes.getSize();
}

const SharedPtr<JsonNode>& JsonNodeContainer::operator[](unsigned index) const {
    return _jsonNodes[index];
}

SharedPtr<JsonNode>& JsonNodeContainer::operator[](unsigned index) {
    return _jsonNodes[index];
}

void JsonNodeContainer::addJsonNode(JsonNodeType type, std::ifstream& in) {
    addJsonNode(type, in, getSize());
}

void JsonNodeContainer::addJsonNode(const SharedPtr<JsonNode>& newJsonNode) {
    addJsonNode(newJsonNode, getSize());
}

void JsonNodeContainer::addJsonNode(SharedPtr<JsonNode>&& newJsonNode) {
    addJsonNode(std::move(newJsonNode), getSize());
}

void JsonNodeContainer::addJsonNode(JsonNodeType type, std::ifstream& in, size_t index) {
    _jsonNodes.pushAt(JsonNodeFactory::create(in, type), index);
}

void JsonNodeContainer::addJsonNode(const SharedPtr<JsonNode>& newJsonNode, size_t index) {
    _jsonNodes.pushAt(newJsonNode, index);
}

void JsonNodeContainer::addJsonNode(SharedPtr<JsonNode>&& newJsonNode, size_t index) {
    _jsonNodes.pushAt(newJsonNode, index);
}

void JsonNodeContainer::addEmptyJsonObject() {
    addEmptyJsonObject(getSize());
}

void JsonNodeContainer::addEmptyJsonArray() {
    addEmptyJsonArray(getSize());
}

void JsonNodeContainer::addEmptyJsonObject(size_t index) {
    _jsonNodes.pushAt(JsonNodeFactory::createEmptyJsonObject(), index);
}

void JsonNodeContainer::addEmptyJsonArray(size_t index) {
    _jsonNodes.pushAt(JsonNodeFactory::createEmptyJsonArray(), index);
}

void JsonNodeContainer::addJsonString(const String& str) {
    addJsonString(str, getSize());
}

void JsonNodeContainer::addJsonString(String&& str) {
    addJsonString(std::move(str), getSize());
}

void JsonNodeContainer::addJsonString(const String& str, size_t index) {
    _jsonNodes.pushAt(JsonNodeFactory::createJsonString(str), index);
}

void JsonNodeContainer::addJsonString(String&& str, size_t index) {
    _jsonNodes.pushAt(JsonNodeFactory::createJsonString(std::move(str)), index);
}

void JsonNodeContainer::addJsonKeyword(const String& keyword) {
    addJsonKeyword(keyword, getSize());
}

void JsonNodeContainer::addJsonKeyword(String&& keyword) {
    addJsonKeyword(std::move(keyword), getSize());
}

void JsonNodeContainer::addJsonKeyword(const String& keyword, size_t index) {
    _jsonNodes.pushAt(JsonNodeFactory::createJsonKeyword(keyword), index);
}

void JsonNodeContainer::addJsonKeyword(String&& keyword, size_t index) {
    _jsonNodes.pushAt(JsonNodeFactory::createJsonKeyword(std::move(keyword)), index);
}

void JsonNodeContainer::addJsonNumber(const String& number) {
    addJsonNumber(number, getSize());
}

void JsonNodeContainer::addJsonNumber(String&& number) {
    addJsonNumber(std::move(number), getSize());
}

void JsonNodeContainer::addJsonNumber(const String& number, size_t index) {
    _jsonNodes.pushAt(JsonNodeFactory::createJsonNumber(number), index);
}

void JsonNodeContainer::addJsonNumber(String&& number, size_t index) {
    _jsonNodes.pushAt(JsonNodeFactory::createJsonNumber(std::move(number)), index);
}

SharedPtr<JsonNode> JsonNodeContainer::removeJsonNodeByIndex(unsigned index) {
    return _jsonNodes.popAt(index);
}
