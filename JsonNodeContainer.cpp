#include "JsonNodeContainer.h"
#include "JsonObjectFactory.h"
#include "JsonArrayFactory.h"
#include "JsonValueFactory.h"
#include "JsonStringFactory.h"

unsigned JsonNodeContainer::getSize() const {
    return _jsonNodes.getSize();
}

SharedPtr<JsonNode> JsonNodeContainer::operator[](unsigned index) const {
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
    JsonObjectFactory objectFactory;
    JsonArrayFactory arrayFactory;
    JsonValueFactory valueFactory;
    JsonStringFactory stringFactory;

    switch(type) {

        case JsonNodeType::JSON_OBJECT:
            _jsonNodes.pushAt(objectFactory.create(in), index);
            break;

        case JsonNodeType::JSON_ARRAY:
            _jsonNodes.pushAt(arrayFactory.create(in), index);
            break;

        case JsonNodeType::JSON_VALUE:
            _jsonNodes.pushAt(valueFactory.create(in), index);
            break;

        case JsonNodeType::JSON_STRING:
            _jsonNodes.pushAt(stringFactory.create(in), index);
            break;
    }
}

void JsonNodeContainer::addJsonNode(const SharedPtr<JsonNode>& newJsonNode, size_t index) {
    _jsonNodes.pushAt(newJsonNode, index);
}

void JsonNodeContainer::addJsonNode(SharedPtr<JsonNode>&& newJsonNode, size_t index) {
    _jsonNodes.pushAt(newJsonNode, index);
}

void JsonNodeContainer::addJsonString(const String& value) {
    addJsonString(value, getSize());
}

void JsonNodeContainer::addJsonString(String&& value) {
    addJsonString(std::move(value), getSize());
}

void JsonNodeContainer::addJsonString(const char* value) {
    addJsonString(value, getSize());
}

void JsonNodeContainer::addJsonString(const String& value, size_t index) {
    _jsonNodes.pushAt(JsonStringFactory::create(value), index);
}

void JsonNodeContainer::addJsonString(String&& value, size_t index) {
    _jsonNodes.pushAt(JsonStringFactory::create(std::move(value)), index);
}

void JsonNodeContainer::addJsonString(const char* value, size_t index) {
    _jsonNodes.pushAt(JsonStringFactory::create(value), index);
}

SharedPtr<JsonNode> JsonNodeContainer::removeJsonNodeByIndex(unsigned index) {
    return _jsonNodes.popAt(index);
}
