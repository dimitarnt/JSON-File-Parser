#include "JsonObject.h"
#include "JsonString.h"
#include "JsonArray.h"
#include "InvalidJsonSyntax.h"
#include <cstring>

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

        JsonNodeType nodeType = _jsonNodeCollection.getTypeByIndex(i);

        if(nodeType == JsonNodeType::JSON_OBJECT) {
            auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[i].operator->();

            jsonObjectPtr->search(searchResults, keyStr);
        }

        if(nodeType == JsonNodeType::JSON_ARRAY) {
            auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[i].operator->();

            jsonArrayPtr->search(searchResults, keyStr);
        }
    }
}

long long JsonObject::findKeyIndex(const String& key) const {

    for(unsigned i = 0; i < _correspondingKeys.getSize(); ++i) {

        if(key == _correspondingKeys[i]) {
            return i;
        }
    }

    return -1;
}

void JsonObject::assertKey(const char* key) {
    size_t keyLength = strlen(key);

    if(getCharCount(key, keyLength, '\"') != 0 || getCharCount(key, keyLength, '\n') != 0) {
        throw InvalidJsonSyntax("Disallowed character in given string");
    }
}

void JsonObject::assertNewKey(const char* newKey) const {
    assertKey(newKey);

    for(unsigned i = 0; i < _correspondingKeys.getSize(); ++i) {

        if(strcmp(newKey, _correspondingKeys[i].getData()) == 0) {
            throw std::invalid_argument("Given key matches at least one other key in current object");
        }
    }
}

void JsonObject::set(const char* path, const char* newStr, unsigned nestingLevel) {
    String key = getKeyInPath(path, nestingLevel);
    assertKey(key.getData());

    long long keyIndex = findKeyIndex(key);

    if(keyIndex == -1) {
        String message("Invalid key at nesting level ");
        message += nestingLevel;

        throw std::invalid_argument(message.getData());
    }

    if(nestingLevel == lastNestingLevelInPath(path)) {
        _jsonNodeCollection[keyIndex] = new JsonString(String(newStr));
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection.getTypeByIndex(keyIndex);

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[keyIndex].operator->();

        jsonObjectPtr->set(path, newStr, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[keyIndex].operator->();

        jsonArrayPtr->set(path, newStr, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_VALUE) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::create(const char* path, bool isAddressingStartingNode, bool createInArray,
                        const char* newKey, const char* newStr, unsigned nestingLevel) {

    if(isAddressingStartingNode || ((nestingLevel - 1) == lastNestingLevelInPath(path) && !createInArray)) {
        assertNewKey(newKey);

        _correspondingKeys.pushBack(String(newKey));
        _jsonNodeCollection.addJsonNode(new JsonString(newStr));
        return;
    }

    String key = getKeyInPath(path, nestingLevel);
    assertKey(key.getData());

    long long keyIndex = findKeyIndex(key);

    if(keyIndex == -1) {
        String message("Invalid key at nesting level ");
        message += nestingLevel;

        throw std::invalid_argument(message.getData());
    }

    JsonNodeType nodeType = _jsonNodeCollection.getTypeByIndex(keyIndex);

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[keyIndex].operator->();

        jsonObjectPtr->create(path, false, createInArray, newKey, newStr, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[keyIndex].operator->();

        jsonArrayPtr->create(path, false, createInArray, newKey, newStr, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_VALUE) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::remove(const char* path, unsigned nestingLevel) {
    String key = getKeyInPath(path, nestingLevel);
    assertKey(key.getData());

    long long keyIndex = findKeyIndex(key);

    if(keyIndex == -1) {
        String message("Invalid key at nesting level ");
        message += nestingLevel;

        throw std::invalid_argument(message.getData());
    }

    if(nestingLevel == lastNestingLevelInPath(path)) {
        _correspondingKeys.popAt(keyIndex);
        _jsonNodeCollection.removeJsonNodeByIndex(keyIndex);
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection.getTypeByIndex(keyIndex);

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[keyIndex].operator->();

        jsonObjectPtr->remove(path, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[keyIndex].operator->();

        jsonArrayPtr->remove(path, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_VALUE) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}
