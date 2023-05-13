#include "JsonValue.h"

JsonValue::JsonValue(std::ifstream& in, bool isString) {
    _value = parseValue(in);
    _isString = isString;
}

void JsonValue::print() const {
    std::cout << ' ';

    if(_isString) {
        std::cout << '\"' << _value << '\"';
        return;
    }

    std::cout << _value;
}

JsonNode* JsonValue::clone() const {
    return new JsonValue(*this);
}
