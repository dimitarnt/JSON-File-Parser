#include "JsonValue.h"

JsonValue::JsonValue(std::ifstream& in) : JsonNode(JsonNodeType::JSON_VALUE) {
    _value = parseValue(in);
}

void JsonValue::print(unsigned nestingLevel) const {
    std::cout << _value;
}

JsonNode* JsonValue::clone() const {
    return new JsonValue(*this);
}
