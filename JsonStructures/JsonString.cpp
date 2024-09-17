#include "JsonString.h"
#include "InvalidJsonSyntax.h"

void JsonString::assertString(const String& str) {

    if(str.isEmpty() || str.getCharCount('\"') != 0 || str.getCharCount('\n') != 0 || str.getCharCount('\t') != 0
       || str.getCharCount(COMMAND_PARAMETER_STARTING_DELIMITER) != 0 || str.getCharCount(COMMAND_PARAMETER_ENDING_DELIMITER) != 0) {

        throw InvalidJsonSyntax("Disallowed character in given string");
    }
}

JsonString::JsonString(std::ifstream& in) : JsonNode(JsonNodeType::JSON_STRING), _str(parseValue(in)) {
    assertString(_str);
}

JsonString::JsonString(const String& str) : JsonNode(JsonNodeType::JSON_STRING) {
    assertString(str);

    _str = str;
}

JsonString::JsonString(String&& str) : JsonNode(JsonNodeType::JSON_STRING) {
    assertString(str);

    _str = std::move(str);
}

void JsonString::print(unsigned nestingLevel) const {
    std::cout << '\"' << _str << '\"';
}

void JsonString::save(std::ofstream& out, unsigned nestingLevel) const {
    out << '\"' << _str << '\"';
}

JsonNode* JsonString::clone() const {
    return new JsonString(*this);
}
