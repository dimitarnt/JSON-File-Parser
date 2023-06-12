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
    char currentChar = 0;

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

            while(true) {
                currentChar = (char)in.peek();

                if(currentChar == ':' || currentChar == ',' || currentChar == '}' || currentChar == ']') {
                    break;
                }

                in.get();
            }

            return result;

        case '\"':
            in.get();
            getCharactersUntilDelimiter(in, result, BUFFER_SIZE, '\"');

            while(true) {
                currentChar = (char)in.peek();

                if(currentChar == ':' || currentChar == ',' || currentChar == '}' || currentChar == ']') {
                    break;
                }

                in.get();
            }

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

unsigned JsonNode::lastNestingLevelInPath(const char* path) {
    size_t pathLength = strlen(path);

    return getCharCount(path, pathLength, '/');
}

String JsonNode::getKeyInPath(const char* path, unsigned nestingLevel) {
    if(nestingLevel > lastNestingLevelInPath(path)) {
        throw std::logic_error("Invalid nesting level for given path");
    }

    String key;
    size_t pathLength = strlen(path);
    size_t startIndex = 0;

    if(nestingLevel != 0) {
        startIndex = getPositionOfChar(path, pathLength, '/', nestingLevel) + 1;
    }

    for(size_t i = startIndex; path[i] != '/' && path[i] != '\0'; ++i) {
        key += path[i];
    }

    return key;
}
