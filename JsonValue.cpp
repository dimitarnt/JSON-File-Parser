#include "JsonValue.h"

JsonValue::JsonValue(std::ifstream& in) {
    setData(in);
}

void JsonValue::setData(std::ifstream& in) {
    _value = parseValue(in);
}

JsonNode* JsonValue::clone() const {
    return new JsonValue(*this);
}
