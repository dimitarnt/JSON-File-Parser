#include "JsonKeyword.h"

void JsonKeyword::assertKeyword(const String& keyword) {

    if(keyword != "true" && keyword != "false" && keyword != "null") {
        throw std::invalid_argument("Invalid keyword");
    }
}

JsonKeyword::JsonKeyword(std::ifstream& in) : JsonNode(JsonNodeType::JSON_KEYWORD), _keyword(parseValue(in)) {
    assertKeyword(_keyword);
}

JsonKeyword::JsonKeyword(const String& keyword) : JsonNode(JsonNodeType::JSON_KEYWORD) {
    assertKeyword(keyword);

    _keyword = keyword;
}

JsonKeyword::JsonKeyword(String&& keyword) : JsonNode(JsonNodeType::JSON_KEYWORD) {
    assertKeyword(keyword);

    _keyword = std::move(keyword);
}

void JsonKeyword::print(unsigned nestingLevel) const {
    std::cout << _keyword;
}

void JsonKeyword::save(std::ofstream& out, unsigned nestingLevel) const {
    out << _keyword;
}

JsonNode* JsonKeyword::clone() const {
    return new JsonKeyword(*this);
}
