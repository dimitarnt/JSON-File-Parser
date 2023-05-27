#include "JsonString.h"

JsonString::JsonString(std::ifstream& in) : JsonNode(JsonNodeType::JSON_STRING) {
    _value = parseValue(in);
}

JsonString::JsonString(const String& value) : JsonNode(JsonNodeType::JSON_STRING), _value(value) {}

JsonString::JsonString(String&& value) : JsonNode(JsonNodeType::JSON_STRING), _value(std::move(value)) {}

void JsonString::print(unsigned nestingLevel, bool isInArray) const {
    std::cout << '\"' << _value << '\"';
}

JsonNode* JsonString::clone() const {
    return new JsonString(*this);
}
