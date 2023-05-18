#include "JsonObject.h"
#include "JsonValue.h"
#include "JsonArray.h"
#include "fileFunctions.h"

JsonObject::JsonObject(std::ifstream& in) : JsonNode(JsonNodeType::JSON_OBJECT) {

    while(true) {
        while(in.peek() != '\"') {
            in.get();
        }

        _correspondingKeys.pushBack(parseValue(in));

        while(true) {
            char currentChar = (char)in.peek();

            if (currentChar == '\"') {

                _jsonNodeCollection.addJsonNode(JsonNodeType::JSON_STRING, in);

                in.get(currentChar);

                if(currentChar == ',') {
                    break;
                }
                if(currentChar == '}') {
                    return;
                }
            }

            if (currentChar == '-' || (currentChar >= '0' && currentChar <= '9')
                || currentChar == 't' || currentChar == 'f' || currentChar == 'n') {

                _jsonNodeCollection.addJsonNode(JsonNodeType::JSON_VALUE, in);

                in.get(currentChar);

                if(currentChar == ',') {
                    break;
                }
                if(currentChar == '}') {
                    return;
                }
            }

            if (currentChar == '{') {
                in.get();

                _jsonNodeCollection.addJsonNode(JsonNodeType::JSON_OBJECT, in);

                while(true) {
                    currentChar = (char)in.get();

                    if(currentChar == ',') {
                        break;
                    }
                    if(currentChar == '}') {
                        return;
                    }
                }
                break;
            }

            if (currentChar == '[') {
                in.get();

                _jsonNodeCollection.addJsonNode(JsonNodeType::JSON_ARRAY, in);

                while(true) {
                    currentChar = (char)in.get();

                    if(currentChar == ',') {
                        break;
                    }
                    if(currentChar == '}') {
                        return;
                    }
                }
                break;
            }

            in.get();
        }
    }
}

void JsonObject::print(unsigned nestingLevel) const {
    size_t numberOfPairsInObject = _correspondingKeys.getSize();

    std::cout << '{' << '\n';

    for(unsigned i = 0; i < numberOfPairsInObject; ++i) {

        printIndentation(nestingLevel + 1);

        std::cout << '\"' << _correspondingKeys[i] << '\"' << ':' << ' ';

        _jsonNodeCollection[i]->print(nestingLevel + 1);

        if(i == numberOfPairsInObject - 1) {
            std::cout << '\n';

            printIndentation(nestingLevel);

            std::cout << '}';
            return;
        }

        std::cout << ',' << '\n';
    }
}

JsonNode* JsonObject::clone() const {
    return new JsonObject(*this);
}
