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

    return *this;
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

const JsonNode* JsonNodeCollection::operator[](unsigned index) const {
    return _jsonNodes[index];
}

JsonNodeType JsonNodeCollection::getTypeByIndex(unsigned index) const {
    return _jsonNodes[index]->getType();
}

void JsonNodeCollection::addJsonNode(JsonNodeType type, std::ifstream& in) {
    _jsonNodes.pushBack(jsonNodeFactory(type, in));
}

JsonNode* jsonNodeFactory(JsonNodeType type, std::ifstream& in) {
    switch(type) {
        case JsonNodeType::JSON_OBJECT:
            return new JsonObject(in);

        case JsonNodeType::JSON_ARRAY:
            return new JsonArray(in);

        case JsonNodeType::JSON_STRING:
            return new JsonString(in);

        case JsonNodeType::JSON_VALUE:
            return new JsonValue(in);
    }

    return nullptr;
}
