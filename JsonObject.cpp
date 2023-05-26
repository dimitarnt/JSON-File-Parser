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

JsonNode* JsonObject::clone() const {
    return new JsonObject(*this);
}

void JsonObject::print(unsigned nestingLevel, bool isInArray) const {
    size_t numberOfPairsInObject = _correspondingKeys.getSize();

    std::cout << '{' << '\n';

    for(unsigned i = 0; i < numberOfPairsInObject; ++i) {

        printIndentation(nestingLevel + 1);

        std::cout << '\"' << _correspondingKeys[i] << '\"' << ':' << ' ';

        _jsonNodeCollection[i]->print(nestingLevel + 1, false);

        if(i == numberOfPairsInObject - 1) {
            std::cout << '\n';

            isInArray ? printIndentation(nestingLevel + 1) : printIndentation(nestingLevel);

            std::cout << '}';
            return;
        }

        std::cout << ',' << '\n';
    }
}

void JsonObject::search(JsonArray& searchResults, const String& keyStr) const {

    for(unsigned i = 0; i < _correspondingKeys.getSize(); ++i) {

        if(_correspondingKeys[i] == keyStr) {
            searchResults.add(_jsonNodeCollection[i]);
        }

        if(_jsonNodeCollection.getTypeByIndex(i) == JsonNodeType::JSON_OBJECT
           ||_jsonNodeCollection.getTypeByIndex(i) == JsonNodeType::JSON_ARRAY) {

            _jsonNodeCollection[i]->search(searchResults, keyStr);
        }
    }
}
