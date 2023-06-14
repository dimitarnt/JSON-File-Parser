#include "JsonArray.h"
#include "JsonObject.h"
#include "JsonString.h"
#include "JsonStringFactory.h"

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

bool JsonArray::isEmpty() const {

    return _jsonNodeCollection.getSize() == 0;
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

void JsonArray::save(std::ofstream& out, unsigned nestingLevel, bool isInArray) const {
    size_t numberOfElementsInArray = _jsonNodeCollection.getSize();

    out << '[' << '\n';

    for(unsigned i = 0; i < numberOfElementsInArray; ++i) {

        putIndentationInFile(out, nestingLevel + 1);

        _jsonNodeCollection[i]->save(out, nestingLevel, true);

        if(i == numberOfElementsInArray - 1) {
            out << '\n';

            isInArray ? putIndentationInFile(out, nestingLevel + 1) : putIndentationInFile(out, nestingLevel);

            out << ']';
            return;
        }

        out << ',' << '\n';
    }
}

void JsonArray::search(JsonArray& searchResults, const String& keyStr) const {

    for(unsigned i = 0; i < _jsonNodeCollection.getSize(); ++i) {
        JsonNodeType nodeType = _jsonNodeCollection[i]->getType();

        if(nodeType == JsonNodeType::JSON_OBJECT) {
            auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[i].get();

            jsonObjectPtr->search(searchResults, keyStr);
        }

        if(nodeType == JsonNodeType::JSON_ARRAY) {
            auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[i].get();

            jsonArrayPtr->search(searchResults, keyStr);
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

void JsonArray::set(const char* path, const char* newStr, unsigned nestingLevel) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        _jsonNodeCollection[index].reset(JsonStringFactory::create(newStr));
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->set(path, newStr, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->set(path, newStr, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_VALUE) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

Pair<String, SharedPtr<JsonNode>> JsonArray::remove(const char* path, unsigned nestingLevel) {
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

        return jsonObjectPtr->remove(path, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        return jsonArrayPtr->remove(path, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_VALUE) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}

void JsonArray::create(const char* path, bool isAddressingStartingNode, bool createInArray,
                       const char* newKey, const char* newStr, unsigned nestingLevel) {

    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertExtendedIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path) && createInArray) {
        _jsonNodeCollection.addJsonString(newStr, index);
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_VALUE || index == _jsonNodeCollection.getSize()) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->create(path, false, createInArray, newKey, newStr, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->create(path, false, createInArray, newKey, newStr, nestingLevel + 1);
    }
}

void JsonArray::move(const char* path, bool isAddressingStartingNode, bool moveInArray,
                     const char* movedKey, SharedPtr<JsonNode>&& jsonNodeForMoving, unsigned int nestingLevel) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertExtendedIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path) && moveInArray) {
        _jsonNodeCollection.addJsonNode(std::move(jsonNodeForMoving), index);
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_VALUE || index == _jsonNodeCollection.getSize()) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->move(path, false, moveInArray, movedKey, std::move(jsonNodeForMoving), nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->move(path, false, moveInArray, movedKey, std::move(jsonNodeForMoving), nestingLevel + 1);
    }
}

void JsonArray::savePath(const char* path, std::ofstream& out, unsigned nestingLevel) {
    String key = getKeyInPath(path, nestingLevel);
    assertNaturalNumberFromStr(key, nestingLevel);

    size_t index = getIndex(std::move(key), nestingLevel);

    assertIndex(index, nestingLevel);

    if(nestingLevel == lastNestingLevelInPath(path)) {
        _jsonNodeCollection[index]->save(out, 0, false);
        return;
    }

    JsonNodeType nodeType = _jsonNodeCollection[index]->getType();

    if(nodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _jsonNodeCollection[index].get();

        jsonObjectPtr->savePath(path, out, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _jsonNodeCollection[index].get();

        jsonArrayPtr->savePath(path, out, nestingLevel + 1);
    }

    if(nodeType == JsonNodeType::JSON_STRING || nodeType == JsonNodeType::JSON_VALUE) {

        throw std::out_of_range("Given path exceeds valid nesting level");
    }
}
