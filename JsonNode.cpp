#include "JsonNode.h"
#include "constants.h"
#include "fileFunctions.h"

String JsonNode::parseValue(std::ifstream& in) const {
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
            getCharactersUntilDelimiter(in, result, BUFFER_SIZE, ',', '}', ']');

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