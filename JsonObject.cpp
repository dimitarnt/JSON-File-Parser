#include "JsonObject.h"
#include "JsonString.h"
#include "JsonArray.h"
#include "InvalidJsonSyntax.h"
#include "JsonStringFactory.h"
#include <cstring>

JsonObject::JsonObject(std::ifstream& in) : JsonNode(JsonNodeType::JSON_OBJECT) {

    while(true) {
        while(in.peek() != '\"') {
            in.get();
        }

        String key(parseValue(in));

        while(true) {
            char currentChar = (char)in.peek();

            if (currentChar == '\"') {

                _jsonPairs.addJsonPair(std::move(key), JsonNodeType::JSON_STRING, in);

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

                _jsonPairs.addJsonPair(std::move(key), JsonNodeType::JSON_VALUE, in);

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

                _jsonPairs.addJsonPair(std::move(key), JsonNodeType::JSON_OBJECT, in);

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

                _jsonPairs.addJsonPair(std::move(key), JsonNodeType::JSON_ARRAY, in);

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
    size_t numberOfPairsInObject = _jsonPairs.getSize();

    std::cout << '{' << '\n';

    for(unsigned i = 0; i < numberOfPairsInObject; ++i) {

        printIndentation(nestingLevel + 1);

        std::cout << '\"' << _jsonPairs.getKey(i) << '\"' << ':' << ' ';

        _jsonPairs.getJsonNode(i)->print(nestingLevel + 1, false);

        if(i == numberOfPairsInObject - 1) {
            std::cout << '\n';

            isInArray ? printIndentation(nestingLevel + 1) : printIndentation(nestingLevel);

            std::cout << '}';
            return;
        }

        std::cout << ',' << '\n';
    }
}

void JsonObject::save(std::ofstream& out, unsigned nestingLevel, bool isInArray) const {
    size_t numberOfPairsInObject = _jsonPairs.getSize();

    out << '{' << '\n';

    for(unsigned i = 0; i < numberOfPairsInObject; ++i) {

        putIndentationInFile(out, nestingLevel + 1);

        out << '\"' << _jsonPairs.getKey(i) << '\"' << ':' << ' ';

        _jsonPairs.getJsonNode(i)->save(out, nestingLevel + 1, false);

        if(i == numberOfPairsInObject - 1) {
            out << '\n';

            isInArray ? putIndentationInFile(out, nestingLevel + 1) : putIndentationInFile(out, nestingLevel);

            out << '}';
            return;
        }

        out << ',' << '\n';
    }
}

void JsonObject::search(JsonArray& searchResults, const String& keyStr) const {

    for(unsigned i = 0; i < _jsonPairs.getSize(); ++i) {

        if(_jsonPairs.getKey(i) == keyStr) {
            searchResults.add(_jsonPairs.getJsonNode(i));
        }

        JsonNodeType nodeType = _jsonPairs.getJsonNode(i)->getType();

        if(nodeType == JsonNodeType::JSON_OBJECT) {
            auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(i).get();

            jsonObjectPtr->search(searchResults, keyStr);
        }

        if(nodeType == JsonNodeType::JSON_ARRAY) {
            auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(i).get();

            jsonArrayPtr->search(searchResults, keyStr);
        }
    }
}

long long JsonObject::findKeyIndex(const String& key) const {

    for(unsigned i = 0; i < _jsonPairs.getSize(); ++i) {

        if(key == _jsonPairs.getKey(i)) {
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

    for(unsigned i = 0; i < _jsonPairs.getSize(); ++i) {

        if(strcmp(newKey, _jsonPairs.getKey(i).getData()) == 0) {
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
        _jsonPairs.accessJsonNode(keyIndex).reset(JsonStringFactory::create(newStr));
        return;
    }

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->set(path, newStr, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->set(path, newStr, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_VALUE) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

Pair<String, SharedPtr<JsonNode>> JsonObject::remove(const char* path, unsigned nestingLevel) {
    String key = getKeyInPath(path, nestingLevel);
    assertKey(key.getData());

    long long keyIndex = findKeyIndex(key);

    if(keyIndex == -1) {
        String message("Invalid key at nesting level ");
        message += nestingLevel;

        throw std::invalid_argument(message.getData());
    }

    if(nestingLevel == lastNestingLevelInPath(path)) {
        return _jsonPairs.removeJsonPairByIndex(keyIndex);
    }

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        return jsonObjectPtr->remove(path, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        return jsonArrayPtr->remove(path, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_VALUE) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::create(const char* path, bool isAddressingStartingNode, bool createInArray,
                        const char* newKey, const char* newStr, unsigned nestingLevel) {

    if(isAddressingStartingNode || ((nestingLevel - 1) == lastNestingLevelInPath(path) && !createInArray)) {
        assertNewKey(newKey);

        _jsonPairs.addJsonStringPair(newKey, newStr);
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

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->create(path, false, createInArray, newKey, newStr, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->create(path, false, createInArray, newKey, newStr, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_VALUE) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::move(const char* path, bool isAddressingStartingNode, bool moveInArray,
                      const char* movedKey, SharedPtr<JsonNode>&& jsonNodeForMoving, unsigned nestingLevel) {

    if(isAddressingStartingNode || ((nestingLevel - 1) == lastNestingLevelInPath(path) && !moveInArray)) {
        _jsonPairs.addJsonPair(movedKey, std::move(jsonNodeForMoving));
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

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->move(path, false, moveInArray, movedKey, std::move(jsonNodeForMoving), nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->move(path, false, moveInArray, movedKey, std::move(jsonNodeForMoving), nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_VALUE) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::savePath(const char* path, std::ofstream& out, unsigned nestingLevel) {
    String key = getKeyInPath(path, nestingLevel);
    assertKey(key.getData());

    long long keyIndex = findKeyIndex(key);

    if(keyIndex == -1) {
        String message("Invalid key at nesting level ");
        message += nestingLevel;

        throw std::invalid_argument(message.getData());
    }

    if(nestingLevel == lastNestingLevelInPath(path)) {
        _jsonPairs.getJsonNode(keyIndex)->save(out, 0, false);
        return;
    }

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->savePath(path, out, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->savePath(path, out, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_VALUE) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}
