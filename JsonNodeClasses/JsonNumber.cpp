#include "JsonNumber.h"

void JsonNumber::assertNumber(const String& number) {

    if(!number.isNumber()) {
        throw std::invalid_argument("Invalid number");
    }
}

JsonNumber::JsonNumber(std::ifstream& in) : JsonNode(JsonNodeType::JSON_NUMBER), _number(parseValue(in)) {
    assertNumber(_number);
}

JsonNumber::JsonNumber(const String& number) : JsonNode(JsonNodeType::JSON_NUMBER) {
    assertNumber(number);

    _number = number;
}

JsonNumber::JsonNumber(String&& number) : JsonNode(JsonNodeType::JSON_NUMBER) {
    assertNumber(number);

    _number = std::move(number);
}

void JsonNumber::print(unsigned nestingLevel) const {
    std::cout << _number;
}

void JsonNumber::save(std::ofstream& out, unsigned nestingLevel) const {
    out << _number;
}

JsonNode* JsonNumber::clone() const {
    return new JsonNumber(*this);
}
