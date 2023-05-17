#include "JsonNodeCollection.h"
#include "JsonObject.h"
#include "JsonArray.h"
#include "JsonString.h"
#include "JsonValue.h"

JsonNodeCollection::JsonNodeCollection(const JsonNodeCollection& other) : _jsonNodes(other._jsonNodes) {
    copyFrom(other);
}

JsonNodeCollection::JsonNodeCollection(JsonNodeCollection&& other)  noexcept : _jsonNodes(std::move(other._jsonNodes)) {}

JsonNodeCollection& JsonNodeCollection::operator=(const JsonNodeCollection& other) {
    if (this != &other) {
        _jsonNodes.operator=(other._jsonNodes);
        free();
        copyFrom(other);
    }

    return *this;
}

JsonNodeCollection::~JsonNodeCollection() {
    free();
}

JsonNodeCollection& JsonNodeCollection::operator=(JsonNodeCollection&& other) noexcept {
    _jsonNodes.operator=(std::move(other._jsonNodes));
}

void JsonNodeCollection::copyFrom(const JsonNodeCollection& other) {

    for (unsigned i = 0; i < _jsonNodes.getSize(); ++i) {
        this->_jsonNodes[i] = other._jsonNodes[i]->clone();
    }
}

void JsonNodeCollection::free() {

    for (unsigned i = 0; i < _jsonNodes.getSize(); ++i) {
        delete _jsonNodes[i];
    }
}

unsigned JsonNodeCollection::getSize() const {
    return _jsonNodes.getSize();
}

void JsonNodeCollection::addJsonNode(JsonNode* newJsonNode) {
    _jsonNodes.pushBack(newJsonNode);
}

void JsonNodeCollection::addJsonObject(std::ifstream& in, unsigned nestingLevel) {
    addJsonNode(new JsonObject(in, nestingLevel));
}

void JsonNodeCollection::addJsonArray(std::ifstream& in, unsigned nestingLevel) {
    addJsonNode(new JsonArray(in, nestingLevel));
}

void JsonNodeCollection::addJsonString(std::ifstream& in) {
    addJsonNode(new JsonString(in));
}

void JsonNodeCollection::addJsonValue(std::ifstream& in) {
    addJsonNode(new JsonValue(in));
}

const JsonNode* JsonNodeCollection::operator[](unsigned index) const {
    return _jsonNodes[index];
}
