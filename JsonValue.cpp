#include "JsonValue.h"

JsonValue::JsonValue(std::ifstream& in) : JsonNode(JsonNodeType::JSON_VALUE) {
    _value = parseValue(in);
}

void JsonValue::print(unsigned nestingLevel, bool isInArray) const {
    std::cout << _value;
}

void JsonValue::save(std::ofstream& out, unsigned nestingLevel, bool isInArray) const {
    out << _value;
}

JsonNode* JsonValue::clone() const {
    return new JsonValue(*this);
}
