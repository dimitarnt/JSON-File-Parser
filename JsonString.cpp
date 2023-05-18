#include "JsonString.h"

JsonString::JsonString(std::ifstream& in) : JsonNode(JsonNodeType::JSON_STRING) {
    _value = parseValue(in);
}

void JsonString::print(unsigned nestingLevel) const {
    std::cout << '\"' << _value << '\"';
}

JsonNode* JsonString::clone() const {
    return new JsonString(*this);
}
