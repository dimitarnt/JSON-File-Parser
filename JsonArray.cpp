#include "JsonArray.h"
#include "JsonValue.h"
#include "fileFunctions.h"

JsonArray::JsonArray() : JsonNode(JsonNodeType::JSON_ARRAY) {}

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

JsonNode* JsonArray::clone() const {
    return new JsonArray(*this);
}

void JsonArray::add(const SharedPtr<JsonNode>& newJsonNode) {
    _jsonNodeCollection.addJsonNode(newJsonNode);
}

void JsonArray::print(unsigned nestingLevel, bool isInArray) const {
    size_t numberOfElementsInArray = _jsonNodeCollection.getSize();

    std::cout << '[' << '\n';

    for(unsigned i = 0; i < numberOfElementsInArray; ++i) {

        printIndentation(nestingLevel + 1);

        _jsonNodeCollection[i]->print(nestingLevel, true);

        if(i == numberOfElementsInArray - 1) {
            std::cout << '\n';

            isInArray ? printIndentation(nestingLevel + 1) : printIndentation(nestingLevel);

            std::cout << ']';
            return;
        }

        std::cout << ',' << '\n';
    }
}

void JsonArray::search(JsonArray& searchResults, const String& keyStr) const {

    for(unsigned i = 0; i < _jsonNodeCollection.getSize(); ++i) {

        if(_jsonNodeCollection.getTypeByIndex(i) == JsonNodeType::JSON_OBJECT
            ||_jsonNodeCollection.getTypeByIndex(i) == JsonNodeType::JSON_ARRAY) {

            _jsonNodeCollection[i]->search(searchResults, keyStr);
        }
    }
}
