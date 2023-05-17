#include "JsonValue.h"

JsonValue::JsonValue(std::ifstream& in) {
    _value = parseValue(in);
}

void JsonValue::print() const {
    std::cout << _value;
}

JsonNode* JsonValue::clone() const {
    return new JsonValue(*this);
}
