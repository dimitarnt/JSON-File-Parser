#include "JsonArray.h"
#include "JsonObject.h"
#include "JsonString.h"
#include "JsonNodeFactory.h"

JsonArray::JsonArray() : JsonNode(JsonNodeType::JSON_ARRAY) {}

JsonArray::JsonArray(std::ifstream& in) : JsonNode(JsonNodeType::JSON_ARRAY) {

    in.get();

    if(in.peek() == ']') {
        in.get();
        return;
    }

    while(true) {
        char currentChar = (char)in.peek();
        bool jsonNodeParsed = false;

        switch(currentChar) {

            case '{':
                _jsonNodeCollection.addJsonNode(JsonNodeType::JSON_OBJECT, in);
                jsonNodeParsed = true;
                break;

            case '[':
                _jsonNodeCollection.addJsonNode(JsonNodeType::JSON_ARRAY, in);
                jsonNodeParsed = true;
                break;

            case '\"':
                _jsonNodeCollection.addJsonNode(JsonNodeType::JSON_STRING, in);
                jsonNodeParsed = true;
                break;

            case 't':
            case 'f':
            case 'n':
                _jsonNodeCollection.addJsonNode(JsonNodeType::JSON_KEYWORD, in);
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
                _jsonNodeCollection.addJsonNode(JsonNodeType::JSON_NUMBER, in);
                jsonNodeParsed = true;
                break;

            default:
                in.get();
                break;
        }

        while(jsonNodeParsed) {
            currentChar = (char)in.get();

            if(currentChar == ',') {
                break;
            }
            if(currentChar == ']') {
                return;
            }
        }

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

bool JsonArray::isEmpty() const {

    return _jsonNodeCollection.getSize() == 0;
}

void JsonArray::print(unsigned nestingLevel) const {
    size_t numberOfElementsInArray = _jsonNodeCollection.getSize();

    if(numberOfElementsInArray == 0) {
        std::cout << "[]";
        return;
    }

    std::cout << '[' << '\n';

    for(unsigned i = 0; i < numberOfElementsInArray; ++i) {

        printIndentation(nestingLevel + 1);

        _jsonNodeCollection[i]->print(nestingLevel + 1);

        if(i == numberOfElementsInArray - 1) {
            std::cout << '\n';

            printIndentation(nestingLevel);

            std::cout << ']';
            return;
        }

        std::cout << ',' << '\n';
    }
}

void JsonArray::save(std::ofstream& out, unsigned nestingLevel) const {
    size_t numberOfElementsInArray = _jsonNodeCollection.getSize();

    if(numberOfElementsInArray == 0) {
        out << "[]";
        return;
    }

    out << '[' << '\n';

    for(unsigned i = 0; i < numberOfElementsInArray; ++i) {

        putIndentationInFile(out, nestingLevel + 1);

        _jsonNodeCollection[i]->save(out, nestingLevel + 1);

        if(i == numberOfElementsInArray - 1) {
            out << '\n';

            putIndentationInFile(out, nestingLevel);

            out << ']';
            return;
        }

        out << ',' << '\n';
    }
}

void JsonArray::search(JsonArray& searchResults, const char* key) const {

    for(unsigned i = 0; i < _jsonNodeCollection.getSize(); ++i) {
        JsonNodeType nodeType = _jsonNodeCollection[i]->getType();

        if(nodeType == JsonNodeType::JSON_OBJECT) {
            auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[i].get();

            jsonObjectPtr->search(searchResults, key);
        }
        else if(nodeType == JsonNodeType::JSON_ARRAY) {
            auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[i].get();

            jsonArrayPtr->search(searchResults, key);
        }
    }
}

unsigned JsonArray::getIndex(const String& key, unsigned nestingLevel) {
    size_t index = 0;
    size_t digitCount = key.getLength();

    for(size_t i = 0; i < digitCount; ++i) {
        index += (key[i] - '0') * pow(10, digitCount - i - 1);
    }

    return index;
}

unsigned JsonArray::getIndex(String&& key, unsigned nestingLevel) {
    size_t index = 0;
    size_t digitCount = key.getLength();

    for(size_t i = 0; i < digitCount; ++i) {
        index += (key[i] - '0') * pow(10, digitCount - i - 1);
    }

    return index;
}

void JsonArray::assertNaturalNumberFromStr(const String &index, unsigned nestingLevel) {
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

void JsonArray::assertExtendedIndex(size_t index, unsigned nestingLevel) const {
    if(index > _jsonNodeCollection.getSize()) {
        String message("Index is out of range at nesting level ");
        message += nestingLevel;

        throw std::invalid_argument(message.getData());
    }
}

void JsonArray::rename(const char* newKey, const char* path, unsigned nestingLevel, UndoActionContainer& undoData) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertIndex(index, nestingLevel);

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->rename(newKey, path, nestingLevel + 1, undoData);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->rename(newKey, path, nestingLevel + 1, undoData);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonArray::set(const SharedPtr<JsonNode>& newNode, const char* path, unsigned nestingLevel, UndoActionContainer& undoData) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        undoData.addSetUndoAction(std::move(_jsonNodeCollection[index]), path);

        _jsonNodeCollection[index] = newNode;
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->set(newNode, path, nestingLevel + 1, undoData);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->set(newNode, path, nestingLevel + 1, undoData);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonArray::set(SharedPtr<JsonNode>&& newNode, const char* path, unsigned nestingLevel, UndoActionContainer& undoData) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        undoData.addSetUndoAction(std::move(_jsonNodeCollection[index]), path);

        _jsonNodeCollection[index] = std::move(newNode);
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->set(std::move(newNode), path, nestingLevel + 1, undoData);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->set(std::move(newNode), path, nestingLevel + 1, undoData);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonArray::setEmptyObject(const char* path, unsigned nestingLevel, UndoActionContainer& undoData) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        undoData.addSetUndoAction(std::move(_jsonNodeCollection[index]), path);

        _jsonNodeCollection[index] = JsonNodeFactory::createEmptyJsonObject();
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->setEmptyObject(path, nestingLevel + 1, undoData);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->setEmptyObject(path, nestingLevel + 1, undoData);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonArray::setEmptyArray(const char* path, unsigned nestingLevel, UndoActionContainer& undoData) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        undoData.addSetUndoAction(std::move(_jsonNodeCollection[index]), path);

        _jsonNodeCollection[index] = JsonNodeFactory::createEmptyJsonArray();
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->setEmptyArray(path, nestingLevel + 1, undoData);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->setEmptyArray(path, nestingLevel + 1, undoData);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonArray::setString(const char* newStr, const char* path, unsigned nestingLevel, UndoActionContainer& undoData) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        undoData.addSetUndoAction(std::move(_jsonNodeCollection[index]), path);

        _jsonNodeCollection[index] = JsonNodeFactory::createJsonString(newStr);
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->setString(newStr, path, nestingLevel + 1, undoData);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->setString(newStr, path, nestingLevel + 1, undoData);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonArray::setKeyword(const char* newKeyword, const char* path, unsigned nestingLevel, UndoActionContainer& undoData) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        undoData.addSetUndoAction(std::move(_jsonNodeCollection[index]), path);

        _jsonNodeCollection[index] = JsonNodeFactory::createJsonKeyword(newKeyword);
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->setKeyword(newKeyword, path, nestingLevel + 1, undoData);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->setKeyword(newKeyword, path, nestingLevel + 1, undoData);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonArray::setNumber(const char* newNumber, const char* path, unsigned nestingLevel, UndoActionContainer& undoData) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        undoData.addSetUndoAction(std::move(_jsonNodeCollection[index]), path);

        _jsonNodeCollection[index] = JsonNodeFactory::createJsonNumber(newNumber);
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->setNumber(newNumber, path, nestingLevel + 1, undoData);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->setNumber(newNumber, path, nestingLevel + 1, undoData);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

JsonPair JsonArray::remove(const char* path, unsigned nestingLevel, UndoActionContainer& undoData) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        undoData.addRemoveUndoActionInArray(std::move(_jsonNodeCollection[index]), path);

        return { String(), _jsonNodeCollection.removeJsonNodeByIndex(index) };
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        return jsonObjectPtr->remove(path, nestingLevel + 1, undoData);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        return jsonArrayPtr->remove(path, nestingLevel + 1, undoData);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonArray::create(const JsonPair& newPair, const char* path, bool createInArray,
                       unsigned nestingLevel) {

    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertExtendedIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path) && createInArray) {
        _jsonNodeCollection.addJsonNode(newPair.getNode(), index);
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_KEYWORD || nodeType == JsonNodeType::JSON_NUMBER
       || index == _jsonNodeCollection.getSize()) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
    else if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->create(newPair, path, createInArray, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->create(newPair, path, createInArray, nestingLevel + 1);
    }
}

void JsonArray::create(JsonPair&& newPair, const char* path, bool createInArray,
                       unsigned nestingLevel) {

    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertExtendedIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path) && createInArray) {
        _jsonNodeCollection.addJsonNode(std::move(newPair.accessNode()), index);
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_KEYWORD || nodeType == JsonNodeType::JSON_NUMBER
       || index == _jsonNodeCollection.getSize()) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
    else if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->create(std::move(newPair), path, createInArray, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->create(std::move(newPair), path, createInArray, nestingLevel + 1);
    }
}

void JsonArray::createEmptyObject(const char* newKey, const char* path, bool createInArray, unsigned nestingLevel) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertExtendedIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path) && createInArray) {
        _jsonNodeCollection.addEmptyJsonObject(index);
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_KEYWORD || nodeType == JsonNodeType::JSON_NUMBER
        || index == _jsonNodeCollection.getSize()) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
    else if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->createEmptyObject(newKey, path, createInArray, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->createEmptyObject(newKey, path, createInArray, nestingLevel + 1);
    }
}

void JsonArray::createEmptyArray(const char* newKey, const char* path, bool createInArray, unsigned nestingLevel) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertExtendedIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path) && createInArray) {
        _jsonNodeCollection.addEmptyJsonArray(index);
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_KEYWORD || nodeType == JsonNodeType::JSON_NUMBER
        || index == _jsonNodeCollection.getSize()) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
    else if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->createEmptyArray(newKey, path, createInArray, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->createEmptyArray(newKey, path, createInArray, nestingLevel + 1);
    }
}

void JsonArray::createString(const char* newKey, const char* newStr, const char* path, bool createInArray, unsigned nestingLevel) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertExtendedIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path) && createInArray) {
        _jsonNodeCollection.addJsonString(newStr, index);
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_KEYWORD || nodeType == JsonNodeType::JSON_NUMBER
        || index == _jsonNodeCollection.getSize()) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
    else if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->createString(newKey, newStr, path, createInArray, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->createString(newKey, newStr, path, createInArray, nestingLevel + 1);
    }
}

void JsonArray::createKeyword(const char* newKey, const char* newKeyword, const char* path, bool createInArray, unsigned nestingLevel) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertExtendedIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path) && createInArray) {
        _jsonNodeCollection.addJsonKeyword(newKeyword, index);
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_KEYWORD || nodeType == JsonNodeType::JSON_NUMBER
        || index == _jsonNodeCollection.getSize()) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
    else if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->createKeyword(newKey, newKeyword, path, createInArray, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->createKeyword(newKey, newKeyword, path, createInArray, nestingLevel + 1);
    }
}

void JsonArray::createNumber(const char* newKey, const char* newNumber, const char* path, bool createInArray, unsigned nestingLevel) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertExtendedIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path) && createInArray) {
        _jsonNodeCollection.addJsonNumber(newNumber, index);
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_KEYWORD || nodeType == JsonNodeType::JSON_NUMBER
       || index == _jsonNodeCollection.getSize()) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
    else if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->createNumber(newKey, newNumber, path, createInArray, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->createNumber(newKey, newNumber, path, createInArray, nestingLevel + 1);
    }
}

JsonPair JsonArray::moveRemove(const char* path, unsigned nestingLevel) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {

        return { String(), _jsonNodeCollection.removeJsonNodeByIndex(index) };
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        return jsonObjectPtr->moveRemove(path, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        return jsonArrayPtr->moveRemove(path, nestingLevel + 1);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonArray::move(JsonPair&& jsonPairForMoving, const char* path, bool moveInArray, unsigned nestingLevel) {

    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertExtendedIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path) && moveInArray) {
        _jsonNodeCollection.addJsonNode(std::move(jsonPairForMoving.accessNode()), index);
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_KEYWORD || nodeType == JsonNodeType::JSON_NUMBER
        || index == _jsonNodeCollection.getSize()) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
    else if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->move(std::move(jsonPairForMoving), path, moveInArray, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->move(std::move(jsonPairForMoving), path, moveInArray, nestingLevel + 1);
    }
}

void JsonArray::printPath(const char* path, unsigned nestingLevel) {

    if(path[0] == '\0') {
        print(0);
        return;
    }

    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        _jsonNodeCollection[index]->print(0);
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->printPath(path, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->printPath(path, nestingLevel + 1);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonArray::savePath(std::ofstream& out, const char* path, unsigned nestingLevel) {

    if(path[0] == '\0') {
        save(out, 0);
        return;
    }

    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        _jsonNodeCollection[index]->save(out, 0);
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->savePath(out, path, nestingLevel + 1);
    }
    else if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->savePath(out, path, nestingLevel + 1);
    }
    else {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}
