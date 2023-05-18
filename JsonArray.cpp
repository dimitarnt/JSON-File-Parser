#include "JsonArray.h"
#include "JsonObject.h"
#include "JsonValue.h"
#include "fileFunctions.h"

JsonArray::JsonArray(std::ifstream& in) : JsonNode(JsonNodeType::JSON_ARRAY) {

    while(true) {
        char currentChar = (char)in.peek();

        if(currentChar == '\"') {

            _jsonNodeCollection.addJsonNode(JsonNodeType::JSON_STRING, in);

            currentChar = (char)in.peek();

            if(currentChar == ']') {
                in.get();
                return;
            }
        }

        if(currentChar == '-' || (currentChar >= '0' && currentChar <= '9')
           || currentChar == 't' || currentChar == 'f' || currentChar == 'n') {

            _jsonNodeCollection.addJsonNode(JsonNodeType::JSON_VALUE, in);

            currentChar = (char)in.peek();

            if(currentChar == ']') {
                in.get();
                return;
            }
        }

        if(currentChar == '{') {
            in.get();

            _jsonNodeCollection.addJsonNode(JsonNodeType::JSON_OBJECT, in);

            while(true) {
                currentChar = (char)in.get();

                if(currentChar == ',') {
                    break;
                }
                if(currentChar == ']') {
                    return;
                }
            }
            continue;
        }

        if(currentChar == '[') {
            in.get();

           _jsonNodeCollection.addJsonNode(JsonNodeType::JSON_ARRAY, in);

            while(true) {
                currentChar = (char)in.get();

                if(currentChar == ',') {
                    break;
                }
                if(currentChar == ']') {
                    return;
                }
            }
            continue;
        }

        in.get();
    }
}

void JsonArray::print(unsigned nestingLevel) const {
    size_t numberOfElementsInArray = _jsonNodeCollection.getSize();

    std::cout << '[' << '\n';

    for(unsigned i = 0; i < numberOfElementsInArray; ++i) {

        printIndentation(nestingLevel + 1);

        _jsonNodeCollection[i]->print(nestingLevel);

        if(i == numberOfElementsInArray - 1) {
            std::cout << '\n';

            printIndentation(nestingLevel);

            std::cout << ']';
            return;
        }

        std::cout << ',' << '\n';
    }
}

JsonNode* JsonArray::clone() const {
    return new JsonArray(*this);
}
