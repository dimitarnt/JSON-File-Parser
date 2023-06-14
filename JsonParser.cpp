#include "JsonParser.h"
#include "JsonArray.h"
#include "JsonObject.h"
#include "InvalidJsonSyntax.h"
#include "constants.h"
#include <cstring>

JsonParser* JsonParser::instance = nullptr;

void JsonParser::assertJsonFileName(const char* fileName) {
    String name(fileName);
    String extension(JSON_FILE_EXTENSION);
    size_t extensionLength = extension.getLength();

    if(name.substring(name.getLength() - extensionLength, extensionLength) != extension) {
        throw std::invalid_argument("File name does not have a JSON file extension");
    }
}

void JsonParser::assertOpenFile() const {
    if(!_fileIsOpened) {
        throw std::runtime_error("There is no open file");
    }
}

JsonParser* JsonParser::getInstance() {
    if(instance == nullptr) {
        instance = new JsonParser();
    }

    return instance;
}

void JsonParser::freeInstance() {
    delete instance;
    instance = nullptr;
}

void JsonParser::openFile(const char* fileName) {
    if(_fileIsOpened) {
        throw std::runtime_error("Close open file before opening a new one");
    }

    _fileIsOpened = true;

    assertJsonFileName(fileName);
    _fileName = String(fileName);

    std::ifstream in(fileName);

    if(!in.is_open()) {
        throw std::ifstream::failure("Unable to open file");
    }

    JsonValidator fileValidator(in);

    fileValidator.validate();

    char firstChar = (char)in.get();

    if(firstChar == '{') {
        _startingNode.addJsonNode(JsonNodeType::JSON_OBJECT, in);
        _startingNodeType = JsonNodeType::JSON_OBJECT;
        return;
    }

    if(firstChar == '[') {
        _startingNode.addJsonNode(JsonNodeType::JSON_ARRAY, in);
        _startingNodeType = JsonNodeType::JSON_ARRAY;
        return;
    }
}

void JsonParser::closeFile() {
    if(!_fileIsOpened) {
        throw std::runtime_error("There is not an open file to close");
    }

    _fileIsOpened = false;
}

void JsonParser::print() const {
    assertOpenFile();

    _startingNode[0]->print(0, false);
    std::cout << std::endl;
}

void JsonParser::search(const char* key) const {
    assertOpenFile();

    JsonArray searchResults;
    String keyStr(key);

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        jsonObjectPtr->search(searchResults, keyStr);
    }

    if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        jsonArrayPtr->search(searchResults, keyStr);
    }

    if(searchResults.isEmpty()) {
        std::cout << '[' << std::endl;
        std::cout << "No results found." << std::endl;
        std::cout << ']' << std::endl;
        return;
    }

    searchResults.print(0, false);
    std::cout << std::endl;
}

void JsonParser::assertString(const char* str) {
    size_t strLength = strlen(str);

    if(getCharCount(str, strLength, '\"') != 0 || getCharCount(str, strLength, '\n') != 0) {
        throw InvalidJsonSyntax("Disallowed character in given string");
    }
}

void JsonParser::set(const char* path, const char* newStr) {
    assertOpenFile();
    assertString(newStr);

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        jsonObjectPtr->set(path, newStr, 0);
    }

    if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        jsonArrayPtr->set(path, newStr, 0);
    }
}

void JsonParser::remove(const char* path) {
    assertOpenFile();

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        jsonObjectPtr->remove(path, 0);
    }

    if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        jsonArrayPtr->remove(path, 0);
    }
}

void JsonParser::create(const char* path, bool isAddressingStartingNode, bool createInArray, const char* newKey, const char* newStr) {
    assertOpenFile();

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        jsonObjectPtr->create(path, isAddressingStartingNode, createInArray, newKey, newStr, 0);
    }

    if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        jsonArrayPtr->create(path, isAddressingStartingNode, createInArray, newKey, newStr, 0);
    }
}

void JsonParser::createInArray(const char* path, const char* newStr) {
    create(path, false, true, "", newStr);
}

void JsonParser::createInStartingObject(const char* newKey, const char* newStr) {
    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        create("", true, false, newKey, newStr);
        return;
    }

    throw std::logic_error("Starting Json Node is not a Json Object");
}

void JsonParser::createInObject(const char* path, const char* newKey, const char* newStr) {
    create(path, false, false, newKey, newStr);
}

void JsonParser::move(const char* pathFrom, const char* pathTo, bool isAddressingStartingNode, bool moveInArray) {

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        Pair<String, SharedPtr<JsonNode>> jsonPairForMoving = jsonObjectPtr->remove(pathFrom, 0);

        if(jsonPairForMoving.getFirst().isEmpty()) {
            jsonObjectPtr->move(pathTo, isAddressingStartingNode, moveInArray, "", std::move(jsonPairForMoving.accessSecond()), 0);
            return;
        }

        jsonObjectPtr->move(pathTo, isAddressingStartingNode, moveInArray, jsonPairForMoving.accessFirst().getData(),
                            std::move(jsonPairForMoving.accessSecond()), 0);
    }

    if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        Pair<String, SharedPtr<JsonNode>> jsonPairForMoving = jsonArrayPtr->remove(pathFrom, 0);

        if(jsonPairForMoving.getFirst().isEmpty()) {
            jsonArrayPtr->move(pathTo, isAddressingStartingNode, moveInArray, "", std::move(jsonPairForMoving.accessSecond()), 0);
            return;
        }

        jsonArrayPtr->move(pathTo, isAddressingStartingNode, moveInArray, jsonPairForMoving.accessFirst().getData(),
                            std::move(jsonPairForMoving.accessSecond()), 0);
    }
}

void JsonParser::moveToArray(const char* pathFrom, const char* pathTo) {
    move(pathFrom, pathTo, false, true);
}

void JsonParser::moveToStartingObject(const char* pathFrom) {
    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        move(pathFrom, "", true, false);
        return;
    }

    throw std::logic_error("Starting Json Node is not a Json Object");
}

void JsonParser::moveToObject(const char* pathFrom, const char* pathTo) {
    move(pathFrom, pathTo, false, false);
}

void JsonParser::save(const char* path, bool isAddressingStartingNode, const char* fileName) const {
    assertOpenFile();
    assertJsonFileName(fileName);

    std::ofstream out(fileName);

    if(!out.is_open()) {
        throw std::ofstream::failure("Unable to open file");
    }

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        if(isAddressingStartingNode) {
            jsonObjectPtr->save(out, 0, false);
            return;
        }

        jsonObjectPtr->savePath(path, out, 0);
    }

    if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        if(isAddressingStartingNode) {
            jsonArrayPtr->save(out, 0, false);
            return;
        }

        jsonArrayPtr->savePath(path, out, 0);
    }
}

void JsonParser::save() const {
    save("", true, _fileName.getData());
}

void JsonParser::save(const char* path) const {
    save(path, false, _fileName.getData());
}

void JsonParser::saveAs(const char* fileName) const {
    if(strcmp(fileName, _fileName.getData()) == 0) {
        throw std::invalid_argument("Given file name matches the one of the currently opened file");
    }

    save("", true, fileName);
}

void JsonParser::saveAs(const char* fileName, const char* path) const {
    if(strcmp(fileName, _fileName.getData()) == 0) {
        throw std::invalid_argument("Given file name matches the one of the currently opened file");
    }

    save(path, false, fileName);
}
