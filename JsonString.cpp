#include "JsonString.h"

JsonString::JsonString(std::ifstream& in) {
    _value = parseValue(in);
}

void JsonString::print() const {
    std::cout << '\"' << _value << '\"';
}

JsonNode* JsonString::clone() const {
    return new JsonString(*this);
}
