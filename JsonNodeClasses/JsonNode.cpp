#include "JsonNode.h"
#include "constants.h"
#include "fileFunctions.h"
#include <cstring>

JsonNode::JsonNode(JsonNodeType type) : _type(type) {}

JsonNodeType JsonNode::getType() const {
    return _type;
}

String JsonNode::parseValue(std::ifstream& in) {
    String result;

    switch(in.peek()) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '-':

        case 't':
        case 'f':
        case 'n':
            getCharactersUntilDelimiter(in, result, BUFFER_SIZE, ' ', '\t', ',', '}', ']');

            return result;

        case '\"':
            in.get();
            getCharactersUntilDelimiter(in, result, BUFFER_SIZE, '\"');

            in.get();

            return result;

        default:
            throw std::logic_error("Json file not validated or wrong usage of function");
    }
}

void JsonNode::printIndentation(unsigned nestingLevel) {

    for(unsigned i = 0; i < nestingLevel; ++i) {

        for(unsigned j = 0; j < NUMBER_OF_INDENTATION_SPACES; ++j) {
            std::cout << ' ';
        }
    }
}

void JsonNode::putIndentationInFile(std::ofstream& out, unsigned nestingLevel) {

    for(unsigned i = 0; i < nestingLevel; ++i) {

        for(unsigned j = 0; j < NUMBER_OF_INDENTATION_SPACES; ++j) {
            out << ' ';
        }
    }
}

unsigned JsonNode::lastNestingLevelInPath(const char* path) {
    String pathStr(path);

    return pathStr.getCharCount('/');
}

String JsonNode::getKeyInPath(const char* path, unsigned nestingLevel) {
    long long pathLastNestingLevel = lastNestingLevelInPath(path);

    if(nestingLevel > pathLastNestingLevel) {
        throw std::logic_error("Invalid nesting level for given path");
    }

    String key;
    String pathStr(path);
    size_t startIndex = 0;

    if(nestingLevel != 0) {
        startIndex = pathStr.getPositionOfChar('/', nestingLevel) + 1;
    }

    for(size_t i = startIndex; path[i] != '/' && path[i] != '\0'; ++i) {
        key += path[i];
    }

    return key;
}
