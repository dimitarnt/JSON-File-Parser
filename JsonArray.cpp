#include "JsonArray.h"
#include "JsonString.h"

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

void JsonArray::add(SharedPtr<JsonNode>&& newJsonNode) {
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

void JsonArray::assertNaturalNumberFromStr(const String &index, unsigned int nestingLevel) {
    if(!index.isNaturalNumber()) {
        String message("Invalid index at nesting level ");
        message += nestingLevel;

        throw std::invalid_argument(message.getData());
    }
}

void JsonArray::assertIndex(size_t index, unsigned nestingLevel) const {
    if(index >= _jsonNodeCollection.getSize()) {
        String message("Index is out of range at nesting level ");
        message += nestingLevel;

        throw std::invalid_argument(message.getData());
    }
}

void JsonArray::set(const char* path, const char* newStr, unsigned nestingLevel) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = 0;
    size_t digitCount = key.getLength();

    for(size_t i = 0; i < digitCount; ++i) {
        index += (key[i] - '0') * pow(10, digitCount - i - 1);
    }

    assertIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        assertString(newStr);

        _jsonNodeCollection[index] = new JsonString(String(newStr));
        return;
    }

    if(_jsonNodeCollection.getTypeByIndex(index) == JsonNodeType::JSON_STRING
       ||_jsonNodeCollection.getTypeByIndex(index) == JsonNodeType::JSON_VALUE) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }

    _jsonNodeCollection[index]->set(path, newStr, nestingLevel + 1);
}

void JsonArray::remove(const char* path, unsigned nestingLevel) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = 0;
    size_t digitCount = key.getLength();

    for(size_t i = 0; i < digitCount; ++i) {
        index += (key[i] - '0') * pow(10, digitCount - i - 1);
    }

    assertIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        _jsonNodeCollection.removeJsonNodeByIndex(index);
        return;
    }

    if(_jsonNodeCollection.getTypeByIndex(index) == JsonNodeType::JSON_STRING
       ||_jsonNodeCollection.getTypeByIndex(index) == JsonNodeType::JSON_VALUE) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }

    _jsonNodeCollection[index]->remove(path, nestingLevel + 1);
}
