#include "JsonNodeCollection.h"
#include "JsonObject.h"
#include "JsonArray.h"
#include "JsonString.h"
#include "JsonValue.h"

unsigned JsonNodeCollection::getSize() const {
    return _jsonNodes.getSize();
}

SharedPtr<JsonNode> JsonNodeCollection::operator[](unsigned index) const {
    return _jsonNodes[index];
}

JsonNodeType JsonNodeCollection::getTypeByIndex(unsigned index) const {
    return _jsonNodes[index]->getType();
}

void JsonNodeCollection::addJsonNode(JsonNodeType type, std::ifstream& in) {
    _jsonNodes.pushBack(jsonNodeFactory(type, in));
}

void JsonNodeCollection::addJsonNode(const SharedPtr<JsonNode>& newJsonNode) {
    _jsonNodes.pushBack(newJsonNode);
}

SharedPtr<JsonNode> jsonNodeFactory(JsonNodeType type, std::ifstream& in) {
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
