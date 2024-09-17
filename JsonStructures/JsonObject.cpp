#include "JsonObject.h"
#include "JsonString.h"
#include "JsonArray.h"
#include "InvalidJsonSyntax.h"
#include "JsonNodeFactory.h"
#include <cstring>

namespace {
    const String DEFAULT_KEY = "Unknown";
}

JsonObject::JsonObject() : JsonNode(JsonNodeType::JSON_OBJECT) {}

JsonObject::JsonObject(std::ifstream& in) : JsonNode(JsonNodeType::JSON_OBJECT) {

    in.get();

    if(in.peek() == '}') {
        in.get();
        return;
    }

    while(true) {
        char currentChar = 0;

        while(true) {
            currentChar = (char)in.peek();

            if(currentChar == '\"') {
                break;
            }

            in.get();
        }

        String key(parseValue(in));

        while(currentChar != ':') {
            currentChar = (char)in.get();
        }

        while(true) {
            currentChar = (char)in.peek();
            bool jsonNodeParsed = false;

            switch (currentChar) {
                case '{':
                    _jsonPairs.addJsonPair(std::move(key), JsonNodeType::JSON_OBJECT, in);
                    jsonNodeParsed = true;
                    break;

                case '[':
                    _jsonPairs.addJsonPair(std::move(key), JsonNodeType::JSON_ARRAY, in);
                    jsonNodeParsed = true;
                    break;

                case '\"':
                    _jsonPairs.addJsonPair(std::move(key), JsonNodeType::JSON_STRING, in);
                    jsonNodeParsed = true;
                    break;

                case 't':
                case 'f':
                case 'n':
                    _jsonPairs.addJsonPair(std::move(key), JsonNodeType::JSON_KEYWORD, in);
                    jsonNodeParsed = true;
                    break;

                case '-':
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
                    _jsonPairs.addJsonPair(std::move(key), JsonNodeType::JSON_NUMBER, in);
                    jsonNodeParsed = true;
                    break;

                default:
                    in.get();
                    break;
            }

            if(jsonNodeParsed) {
                break;
            }
        }

        while(true) {
            currentChar = (char)in.get();

            if(currentChar == ',') {
                break;
            }
            if(currentChar == '}') {
                return;
            }
        }

    }
}

JsonNode* JsonObject::clone() const {
    return new JsonObject(*this);
}

void JsonObject::print(unsigned nestingLevel) const {
    size_t numberOfPairsInObject = _jsonPairs.getSize();

    if(numberOfPairsInObject == 0) {
        std::cout << "{}";
        return;
    }

    std::cout << '{' << '\n';

    for(unsigned i = 0; i < numberOfPairsInObject; ++i) {

        printIndentation(nestingLevel + 1);

        std::cout << '\"' << _jsonPairs.getKey(i) << '\"' << ':' << ' ';

        _jsonPairs.getJsonNode(i)->print(nestingLevel + 1);

        if(i == numberOfPairsInObject - 1) {
            std::cout << '\n';

            printIndentation(nestingLevel);

            std::cout << '}';
            return;
        }

        std::cout << ',' << '\n';
    }

    std::cout << '\n';
}

void JsonObject::save(std::ofstream& out, unsigned nestingLevel) const {
    size_t numberOfPairsInObject = _jsonPairs.getSize();

    if(numberOfPairsInObject == 0) {
        out << "{}";
        return;
    }

    out << '{' << '\n';

    for(unsigned i = 0; i < numberOfPairsInObject; ++i) {

        putIndentationInFile(out, nestingLevel + 1);

        out << '\"' << _jsonPairs.getKey(i) << '\"' << ':' << ' ';

        _jsonPairs.getJsonNode(i)->save(out, nestingLevel + 1);

        if(i == numberOfPairsInObject - 1) {
            out << '\n';

            putIndentationInFile(out, nestingLevel);

            out << '}';
            return;
        }

        out << ',' << '\n';
    }
}

void JsonObject::search(JsonArray& searchResults, const char* key) const {

    for(unsigned i = 0; i < _jsonPairs.getSize(); ++i) {

        if(_jsonPairs.getKey(i) == key) {
            searchResults.add(_jsonPairs.getJsonNode(i));
        }

        JsonNodeType nodeType = _jsonPairs.getJsonNode(i)->getType();

        if(nodeType == JsonNodeType::JSON_OBJECT) {
            auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(i).get();

            jsonObjectPtr->search(searchResults, key);
        }
        else if(nodeType == JsonNodeType::JSON_ARRAY) {
            auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(i).get();

            jsonArrayPtr->search(searchResults, key);
        }
    }
}

unsigned JsonObject::getKeyIndex(const String& key, unsigned nestingLevel) const {
    long long keyIndex = -1;

    for(unsigned i = 0; i < _jsonPairs.getSize(); ++i) {

        if(key == _jsonPairs.getKey(i)) {
            keyIndex = i;
        }
    }

    if(keyIndex == -1) {
        String message("Invalid key at nesting level ");
        message += nestingLevel;

        throw std::invalid_argument(message.getData());
    }

    return keyIndex;
}

unsigned JsonObject::getKeyIndex(String&& key, unsigned nestingLevel) const {
    long long keyIndex = -1;

    for(unsigned i = 0; i < _jsonPairs.getSize(); ++i) {

        if(key == _jsonPairs.getKey(i)) {
            keyIndex = i;
        }
    }

    if(keyIndex == -1) {
        String message("Invalid key at nesting level ");
        message += nestingLevel;

        throw std::invalid_argument(message.getData());
    }

    return keyIndex;
}

void JsonObject::assertUniqueKey(const String& newKey) const {

    for(unsigned i = 0; i < _jsonPairs.getSize(); ++i) {

        if(newKey == _jsonPairs.getKey(i)) {
            throw std::invalid_argument("Given key matches at least one other key in current object");
        }
    }
}

String JsonObject::createDefaultNewKey() const {

    try {
        assertUniqueKey(DEFAULT_KEY);
    }
    catch(const std::invalid_argument& exception) {

        for(size_t i = 1; i <= _jsonPairs.getSize(); ++i) {
            String newKey = DEFAULT_KEY + i;

            try {
                assertUniqueKey(newKey);
            }
            catch(const std::invalid_argument& exception) {
                continue;
            }

            return newKey;
        }
    }

    return DEFAULT_KEY;
}

void JsonObject::rename(const char* newKey, const char* path, unsigned nestingLevel, UndoActionContainer& undoData) {
    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        undoData.addRenameUndoAction(std::move(_jsonPairs.accessKey(keyIndex).accessKeyString()), newKey, path);

        _jsonPairs.setKey(newKey, keyIndex);
        return;
    }

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->rename(newKey, path, nestingLevel + 1, undoData);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->rename(newKey, path, nestingLevel + 1, undoData);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::set(const SharedPtr<JsonNode>& newNode, const char* path, unsigned nestingLevel, UndoActionContainer& undoData) {
    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        undoData.addSetUndoAction(std::move(_jsonPairs.accessJsonNode(keyIndex)), path);

        _jsonPairs.accessJsonNode(keyIndex) = newNode;
        return;
    }

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->set(newNode, path, nestingLevel + 1, undoData);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->set(newNode, path, nestingLevel + 1, undoData);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::set(SharedPtr<JsonNode>&& newNode, const char* path, unsigned nestingLevel, UndoActionContainer& undoData) {
    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        undoData.addSetUndoAction(std::move(_jsonPairs.accessJsonNode(keyIndex)), path);

        _jsonPairs.accessJsonNode(keyIndex) = std::move(newNode);
        return;
    }

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->set(std::move(newNode), path, nestingLevel + 1, undoData);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->set(std::move(newNode), path, nestingLevel + 1, undoData);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::setEmptyObject(const char* path, unsigned nestingLevel, UndoActionContainer& undoData) {
    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        undoData.addSetUndoAction(std::move(_jsonPairs.accessJsonNode(keyIndex)), path);

        _jsonPairs.accessJsonNode(keyIndex) = JsonNodeFactory::createEmptyJsonObject();
        return;
    }

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->setEmptyObject(path, nestingLevel + 1, undoData);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->setEmptyObject(path, nestingLevel + 1, undoData);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::setEmptyArray(const char* path, unsigned nestingLevel, UndoActionContainer& undoData) {
    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        undoData.addSetUndoAction(std::move(_jsonPairs.accessJsonNode(keyIndex)), path);

        _jsonPairs.accessJsonNode(keyIndex) = JsonNodeFactory::createEmptyJsonArray();
        return;
    }

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->setEmptyArray(path, nestingLevel + 1, undoData);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->setEmptyArray(path, nestingLevel + 1, undoData);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::setString(const char* newStr, const char* path, unsigned nestingLevel, UndoActionContainer& undoData) {
    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        undoData.addSetUndoAction(std::move(_jsonPairs.accessJsonNode(keyIndex)), path);

        _jsonPairs.accessJsonNode(keyIndex) = JsonNodeFactory::createJsonString(newStr);
        return;
    }

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->setString(newStr, path, nestingLevel + 1, undoData);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->setString(newStr, path, nestingLevel + 1, undoData);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::setKeyword(const char* newKeyword, const char* path, unsigned nestingLevel, UndoActionContainer& undoData) {
    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        undoData.addSetUndoAction(std::move(_jsonPairs.accessJsonNode(keyIndex)), path);

        _jsonPairs.accessJsonNode(keyIndex) = JsonNodeFactory::createJsonKeyword(newKeyword);
        return;
    }

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->setKeyword(newKeyword, path, nestingLevel + 1, undoData);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->setKeyword(newKeyword, path, nestingLevel + 1, undoData);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::setNumber(const char* newNumber, const char* path, unsigned nestingLevel, UndoActionContainer& undoData) {
    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        undoData.addSetUndoAction(std::move(_jsonPairs.accessJsonNode(keyIndex)), path);

        _jsonPairs.accessJsonNode(keyIndex) = JsonNodeFactory::createJsonNumber(newNumber);
        return;
    }

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->setNumber(newNumber, path, nestingLevel + 1, undoData);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->setNumber(newNumber, path, nestingLevel + 1, undoData);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

JsonPair JsonObject::remove(const char* path, unsigned nestingLevel, UndoActionContainer& undoData) {
    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        undoData.addRemoveUndoActionInObject(std::move(_jsonPairs[keyIndex]), path);

        return _jsonPairs.removeJsonPairByIndex(keyIndex);
    }

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        return jsonObjectPtr->remove(path, nestingLevel + 1, undoData);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        return jsonArrayPtr->remove(path, nestingLevel + 1, undoData);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::create(const JsonPair& newPair, const char* path, bool createInArray, unsigned nestingLevel) {

    if(path[0] == '\0' || ((nestingLevel - 1) == lastNestingLevelInPath(path) && !createInArray)) {
        assertUniqueKey(newPair.getKey());

        if(newPair.getKey().isEmpty()) {
            _jsonPairs.addJsonPair(createDefaultNewKey(), newPair.getNode());
            return;
        }

        _jsonPairs.addJsonPair(newPair.getKey(), newPair.getNode());
        return;
    }

    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->create(newPair, path, createInArray, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->create(newPair, path, createInArray, nestingLevel + 1);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::create(JsonPair&& newPair, const char* path, bool createInArray, unsigned nestingLevel) {

    if(path[0] == '\0' || ((nestingLevel - 1) == lastNestingLevelInPath(path) && !createInArray)) {
        assertUniqueKey(newPair.getKey());

        if(newPair.getKey().isEmpty()) {
            _jsonPairs.addJsonPair(createDefaultNewKey(), std::move(newPair.accessNode()));
            return;
        }

        _jsonPairs.addJsonPair(std::move(newPair.accessKey()), std::move(newPair.accessNode()));
        return;
    }

    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->create(std::move(newPair), path, createInArray, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->create(std::move(newPair), path, createInArray, nestingLevel + 1);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::createEmptyObject(const char* newKey, const char* path, bool createInArray, unsigned nestingLevel) {

    if(path[0] == '\0' || ((nestingLevel - 1) == lastNestingLevelInPath(path) && !createInArray)) {
        assertUniqueKey(newKey);

        if(strcmp(newKey, "") == 0) {
            _jsonPairs.addEmptyJsonObjectPair(createDefaultNewKey());
            return;
        }

        _jsonPairs.addEmptyJsonObjectPair(String(newKey));
        return;
    }

    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->createEmptyObject(newKey, path, createInArray, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->createEmptyObject(newKey, path, createInArray, nestingLevel + 1);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::createEmptyArray(const char* newKey, const char* path, bool createInArray, unsigned nestingLevel) {

    if(path[0] == '\0' || ((nestingLevel - 1) == lastNestingLevelInPath(path) && !createInArray)) {
        assertUniqueKey(newKey);

        if(strcmp(newKey, "") == 0) {
            _jsonPairs.addEmptyJsonArrayPair(createDefaultNewKey());
            return;
        }

        _jsonPairs.addEmptyJsonArrayPair(String(newKey));
        return;
    }

    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->createEmptyArray(newKey, path, createInArray, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->createEmptyArray(newKey, path, createInArray, nestingLevel + 1);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::createString(const char* newKey, const char* newStr, const char* path, bool createInArray, unsigned nestingLevel) {

    if(path[0] == '\0' || ((nestingLevel - 1) == lastNestingLevelInPath(path) && !createInArray)) {
        assertUniqueKey(newKey);

        if(strcmp(newKey, "") == 0) {
            _jsonPairs.addJsonStringPair(createDefaultNewKey(), newStr);
            return;
        }

        _jsonPairs.addJsonStringPair(String(newKey), newStr);
        return;
    }

    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->createString(newKey, newStr, path, createInArray, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->createString(newKey, newStr, path, createInArray, nestingLevel + 1);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::createKeyword(const char* newKey, const char* newKeyword, const char* path, bool createInArray, unsigned nestingLevel) {

    if(path[0] == '\0' || ((nestingLevel - 1) == lastNestingLevelInPath(path) && !createInArray)) {
        assertUniqueKey(newKey);

        if(strcmp(newKey, "") == 0) {
            _jsonPairs.addJsonKeywordPair(createDefaultNewKey(), newKeyword);
            return;
        }

        _jsonPairs.addJsonKeywordPair(String(newKey), newKeyword);
        return;
    }

    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->createKeyword(newKey, newKeyword, path, createInArray, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->createKeyword(newKey, newKeyword, path, createInArray, nestingLevel + 1);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::createNumber(const char* newKey, const char* newNumber, const char* path, bool createInArray, unsigned nestingLevel) {

    if(path[0] == '\0' || ((nestingLevel - 1) == lastNestingLevelInPath(path) && !createInArray)) {
        assertUniqueKey(newKey);

        if(strcmp(newKey, "") == 0) {
            _jsonPairs.addJsonNumberPair(createDefaultNewKey(), newNumber);
            return;
        }

        _jsonPairs.addJsonNumberPair(String(newKey), newNumber);
        return;
    }

    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->createNumber(newKey, newNumber, path, createInArray, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->createNumber(newKey, newNumber, path, createInArray, nestingLevel + 1);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

JsonPair JsonObject::moveRemove(const char* path, unsigned nestingLevel) {
    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {

        return _jsonPairs.removeJsonPairByIndex(keyIndex);
    }

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        return jsonObjectPtr->moveRemove(path, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        return jsonArrayPtr->moveRemove(path, nestingLevel + 1);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::move(JsonPair&& jsonPairForMoving, const char* path, bool moveInArray, unsigned nestingLevel) {

    if(path[0] == '\0' || ((nestingLevel - 1) == lastNestingLevelInPath(path) && !moveInArray)) {
        assertUniqueKey(jsonPairForMoving.getKey());

        if(strcmp(jsonPairForMoving.getKey().getData(), "") == 0) {
            _jsonPairs.addJsonPair(createDefaultNewKey(), std::move(jsonPairForMoving).accessNode());
            return;
        }

        _jsonPairs.addJsonPair(std::move(jsonPairForMoving));
        return;
    }

    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->move(std::move(jsonPairForMoving), path, moveInArray, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->move(std::move(jsonPairForMoving), path, moveInArray, nestingLevel + 1);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::printPath(const char* path, unsigned nestingLevel) {

    if(path[0] == '\0') {
        print(0);
        return;
    }

    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        _jsonPairs.getJsonNode(keyIndex)->print(0);
        return;
    }

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->printPath(path, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->printPath(path, nestingLevel + 1);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonObject::savePath(std::ofstream& out, const char* path, unsigned nestingLevel) {

    if(path[0] == '\0') {
        save(out, 0);
        return;
    }

    String key = getKeyInPath(path, nestingLevel);

    unsigned keyIndex = getKeyIndex(std::move(key), nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        _jsonPairs.getJsonNode(keyIndex)->save(out, 0);
        return;
    }

    JsonNodeType nodeType = _jsonPairs.getJsonNode(keyIndex)->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonObjectPtr->savePath(out, path, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonPairs.getJsonNode(keyIndex).get();

        jsonArrayPtr->savePath(out, path, nestingLevel + 1);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}
