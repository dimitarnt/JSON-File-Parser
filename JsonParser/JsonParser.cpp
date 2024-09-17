#include "JsonParser.h"
#include "JsonArray.h"
#include "JsonObject.h"
#include "InvalidJsonSyntax.h"
#include "constants.h"
#include "fileFunctions.h"
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

void JsonParser::assertKey(const char* key) {

    if(strcmp(key, "") == 0) {
        throw std::invalid_argument("Key cannot be an empty string");
    }
}

void JsonParser::assertOpenFile() const {

    if(!_fileIsOpened) {
        throw std::logic_error("There is no open file");
    }
}

void JsonParser::assertNonInitiatedMove() const {

    if(!_moveFromPath.isEmpty()) {
        throw std::logic_error("Selected element for moving should be moved first");
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

bool JsonParser::fileIsOpened() const {
    return _fileIsOpened;
}

bool JsonParser::changesHaveBeenMade() const {
    return _changesHaveBeenMade;
}

bool JsonParser::actionIsBeingUndone() const {
    return _actionIsBeingUndone;
}

void JsonParser::validate(const char* fileName) {
    std::ifstream in(fileName);

    if(!in.is_open()) {
        throw std::ifstream::failure("Unable to open file");
    }

    JsonValidator fileValidator(in);

    fileValidator.validate();
}

void JsonParser::openFile(const char* fileName) {
    if(_fileIsOpened) {
        throw std::runtime_error("Close open file before opening a new one");
    }

    assertJsonFileName(fileName);
    _fileName = String(fileName);

    std::ifstream in(fileName);

    if(!in.is_open()) {
        throw std::ifstream::failure("Unable to open file");
    }

    if(getFileSize(in) == 0) {
        _startingNode.addEmptyJsonObject();
        _startingNodeType = JsonNodeType::JSON_OBJECT;

        _fileIsOpened = true;
        _changesHaveBeenMade = false;
        return;
    }

    JsonValidator fileValidator(in);

    fileValidator.validate();

    char firstChar = (char)in.peek();

    if(firstChar == '{') {
        _startingNode.addJsonNode(JsonNodeType::JSON_OBJECT, in);
        _startingNodeType = JsonNodeType::JSON_OBJECT;
    }
    else if(firstChar == '[') {
        _startingNode.addJsonNode(JsonNodeType::JSON_ARRAY, in);
        _startingNodeType = JsonNodeType::JSON_ARRAY;
    }

    _fileIsOpened = true;
    _changesHaveBeenMade = false;
}

void JsonParser::closeFile() {
    if(!_fileIsOpened) {
        throw std::runtime_error("There is not an open file to close");
    }

    freeInstance();

    _fileIsOpened = false;
}

void JsonParser::print() const {
    assertOpenFile();

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        jsonObjectPtr->printPath(_traversedPath.getData(), 0);
    }
    else if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        jsonArrayPtr->printPath(_traversedPath.getData(), 0);
    }
}

void JsonParser::search(const char* key) const {
    assertOpenFile();
    assertKey(key);

    JsonArray searchResults;

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        jsonObjectPtr->search(searchResults, key);
    }
    else if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        jsonArrayPtr->search(searchResults, key);
    }

    if(searchResults.isEmpty()) {
        std::cout << '[' << std::endl;
        std::cout << "No results found." << std::endl;
        std::cout << ']' << std::endl;
        return;
    }

    searchResults.print(0);
}

String JsonParser::getConcatenatedTraversalPath(const char* path) const {
    if(_actionIsBeingUndone) {
        return path;
    }

    if(path[0] == '\0') {
        return _traversedPath;
    }

    if(_traversedPath.isEmpty()) {
        return path;
    }

    String result = _traversedPath + '/';
    result += path;

    return result;
}

void JsonParser::traverse(const char* path) {
    assertOpenFile();
    String oldTraversedPath = _traversedPath;

    if(_traversedPath.isEmpty()) {
        _traversedPath += path;
    }
    else {
        _traversedPath += '/';
        _traversedPath += path;
    }

    try {
        print();
    }
    catch(const std::exception& exception) {
        _traversedPath = oldTraversedPath;
        throw;
    }
}

String JsonParser::getPreviousPath(const char* path) {
    String pathStr(path);
    size_t pathLength = pathStr.getLength();

    for(size_t i = 0; i < pathLength; ++i) {

        if(pathStr[pathLength - i - 1] == '/') {
            return pathStr.substring(0, pathLength - i - 1);
        }
    }

    return "";
}

String JsonParser::getBaseKey(const char* path) {
    String pathStr(path);
    size_t pathLength = pathStr.getLength();

    size_t startIndex = 0;

    for(size_t i = 0; i < pathLength; ++i) {

        if(pathStr[pathLength - i - 1] == '/') {
            startIndex = pathLength - i - 1;
        }
    }

    return pathStr.substring(startIndex, pathLength - startIndex);
}

String JsonParser::addToPath(const char* path, const String& newKey) {
    String pathStr(path);

    pathStr += '/';
    pathStr += newKey;

    return pathStr;
}

void JsonParser::previous() {
    assertOpenFile();

    _traversedPath = getPreviousPath(_traversedPath.getData());

    print();
}

void JsonParser::goBack() {
    assertOpenFile();

    _traversedPath.clear();

    print();
}

void JsonParser::undo() {
    _actionIsBeingUndone = true;
    _undoData.lockAdding();

    try {
        _undoData.undo();
    }
    catch(const std::exception& exception) {
        _actionIsBeingUndone = false;
        _undoData.unlockAdding();

        throw;
    }

    _actionIsBeingUndone = false;
    _undoData.unlockAdding();

    _changesHaveBeenMade = true;
}

void JsonParser::undoAll() {
    _actionIsBeingUndone = true;
    _undoData.lockAdding();

    try {
        _undoData.undoAll();
    }
    catch(const std::exception& exception) {
        _actionIsBeingUndone = false;
        _undoData.unlockAdding();

        throw;
    }

    _actionIsBeingUndone = false;
    _undoData.unlockAdding();

    _changesHaveBeenMade = true;
}

void JsonParser::rename(const char* newKey, const char* path) {
    assertOpenFile();
    assertNonInitiatedMove();
    assertKey(newKey);

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        jsonObjectPtr->rename(newKey, getConcatenatedTraversalPath(path).getData(), 0, _undoData);
    }
    else if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        jsonArrayPtr->rename(newKey, getConcatenatedTraversalPath(path).getData(), 0, _undoData);
    }

    _changesHaveBeenMade = true;
}

void JsonParser::set(const SharedPtr<JsonNode>& newNode, const char* path) {
    assertOpenFile();
    assertNonInitiatedMove();

    if(getConcatenatedTraversalPath(path).isEmpty()) {
        throw std::logic_error("Invalid set path");
    }

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        jsonObjectPtr->set(newNode, getConcatenatedTraversalPath(path).getData(), 0, _undoData);
    }
    else if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        jsonArrayPtr->set(newNode, getConcatenatedTraversalPath(path).getData(), 0, _undoData);
    }

    _changesHaveBeenMade = true;
}

void JsonParser::set(SharedPtr<JsonNode>&& newNode, const char* path) {
    assertOpenFile();
    assertNonInitiatedMove();

    if(getConcatenatedTraversalPath(path).isEmpty()) {
        throw std::logic_error("Invalid set path");
    }

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        jsonObjectPtr->set(std::move(newNode), getConcatenatedTraversalPath(path).getData(), 0, _undoData);
    }
    else if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        jsonArrayPtr->set(std::move(newNode), getConcatenatedTraversalPath(path).getData(), 0, _undoData);
    }

    _changesHaveBeenMade = true;
}

void JsonParser::set(const char* newValue, const char* path, JsonNodeType type) {
    assertOpenFile();
    assertNonInitiatedMove();

    if(getConcatenatedTraversalPath(path).isEmpty()) {
        throw std::logic_error("Invalid set path");
    }

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        switch(type) {
            case JsonNodeType::JSON_OBJECT:
                jsonObjectPtr->setEmptyObject(getConcatenatedTraversalPath(path).getData(), 0, _undoData);
                break;

            case JsonNodeType::JSON_ARRAY:
                jsonObjectPtr->setEmptyArray(getConcatenatedTraversalPath(path).getData(), 0, _undoData);
                break;

            case JsonNodeType::JSON_STRING:
                jsonObjectPtr->setString(newValue, getConcatenatedTraversalPath(path).getData(), 0, _undoData);
                break;

            case JsonNodeType::JSON_KEYWORD:
                jsonObjectPtr->setKeyword(newValue, getConcatenatedTraversalPath(path).getData(), 0, _undoData);
                break;

            case JsonNodeType::JSON_NUMBER:
                jsonObjectPtr->setNumber(newValue, getConcatenatedTraversalPath(path).getData(), 0, _undoData);
                break;
        }
    }
    else if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        switch(type) {
            case JsonNodeType::JSON_OBJECT:
                jsonArrayPtr->setEmptyObject(getConcatenatedTraversalPath(path).getData(), 0, _undoData);
                break;

            case JsonNodeType::JSON_ARRAY:
                jsonArrayPtr->setEmptyArray(getConcatenatedTraversalPath(path).getData(), 0, _undoData);
                break;

            case JsonNodeType::JSON_STRING:
                jsonArrayPtr->setString(newValue, getConcatenatedTraversalPath(path).getData(), 0, _undoData);
                break;

            case JsonNodeType::JSON_KEYWORD:
                jsonArrayPtr->setKeyword(newValue, getConcatenatedTraversalPath(path).getData(), 0, _undoData);
                break;

            case JsonNodeType::JSON_NUMBER:
                jsonArrayPtr->setNumber(newValue, getConcatenatedTraversalPath(path).getData(), 0, _undoData);
                break;
        }
    }

    _changesHaveBeenMade = true;
}

void JsonParser::setEmptyObject(const char* path) {
    set("", path, JsonNodeType::JSON_OBJECT);
}

void JsonParser::setEmptyArray(const char* path) {
    set("", path, JsonNodeType::JSON_ARRAY);
}

void JsonParser::setString(const char* newStr, const char* path) {
    set(newStr, path, JsonNodeType::JSON_STRING);
}

void JsonParser::setKeyword(const char* newKeyword, const char* path) {
    set(newKeyword, path, JsonNodeType::JSON_KEYWORD);
}

void JsonParser::setNumber(const char* newNumber, const char* path) {
    set(newNumber, path, JsonNodeType::JSON_NUMBER);
}

void JsonParser::remove(const char* path) {
    assertOpenFile();
    assertNonInitiatedMove();

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        jsonObjectPtr->remove(getConcatenatedTraversalPath(path).getData(), 0, _undoData);
    }
    else if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        jsonArrayPtr->remove(getConcatenatedTraversalPath(path).getData(), 0, _undoData);
    }

    _changesHaveBeenMade = true;
}

void JsonParser::createInArray(const SharedPtr<JsonNode>& newNode, const char* path) {
    assertOpenFile();
    assertNonInitiatedMove();

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        jsonObjectPtr->create( { String(), newNode } , getConcatenatedTraversalPath(path).getData(), true, 0);
    }
    else if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        jsonArrayPtr->create( { String(), newNode } , getConcatenatedTraversalPath(path).getData(), true, 0);
    }

    _changesHaveBeenMade = true;

    _undoData.addCreateUndoAction(getConcatenatedTraversalPath(path).getData());
}

void JsonParser::createInArray(SharedPtr<JsonNode>&& newNode, const char* path) {
    assertOpenFile();
    assertNonInitiatedMove();

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        jsonObjectPtr->create( { String(), std::move(newNode) } , getConcatenatedTraversalPath(path).getData(), true, 0);
    }
    else if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        jsonArrayPtr->create( { String(), std::move(newNode) } , getConcatenatedTraversalPath(path).getData(), true, 0);
    }

    _changesHaveBeenMade = true;

    _undoData.addCreateUndoAction(getConcatenatedTraversalPath(path).getData());
}

void JsonParser::createInObject(const JsonPair& newPair, const char* path) {
    assertOpenFile();
    assertNonInitiatedMove();
    assertKey(newPair.getKey().getData());

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        jsonObjectPtr->create(newPair, getConcatenatedTraversalPath(path).getData(), false, 0);
    }
    else if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        jsonArrayPtr->create(newPair, getConcatenatedTraversalPath(path).getData(), false, 0);
    }

    _changesHaveBeenMade = true;

    _undoData.addCreateUndoAction(getConcatenatedTraversalPath(path).getData());
}

void JsonParser::createInObject(JsonPair&& newPair, const char* path) {
    assertOpenFile();
    assertNonInitiatedMove();
    assertKey(newPair.getKey().getData());

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        jsonObjectPtr->create(std::move(newPair), getConcatenatedTraversalPath(path).getData(), false, 0);
    }
    else if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        jsonArrayPtr->create(std::move(newPair), getConcatenatedTraversalPath(path).getData(), false, 0);
    }

    _changesHaveBeenMade = true;

    _undoData.addCreateUndoAction(getConcatenatedTraversalPath(path).getData());
}

void JsonParser::createInArray(const char* newValue, const char* path, JsonNodeType type) {
    assertOpenFile();
    assertNonInitiatedMove();

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        switch(type) {
            case JsonNodeType::JSON_OBJECT:
                jsonObjectPtr->createEmptyObject("", getConcatenatedTraversalPath(path).getData(), true, 0);
                break;

            case JsonNodeType::JSON_ARRAY:
                jsonObjectPtr->createEmptyArray("", getConcatenatedTraversalPath(path).getData(), true, 0);
                break;

            case JsonNodeType::JSON_STRING:
                jsonObjectPtr->createString("", newValue, getConcatenatedTraversalPath(path).getData(), true, 0);
                break;

            case JsonNodeType::JSON_KEYWORD:
                jsonObjectPtr->createKeyword("", newValue, getConcatenatedTraversalPath(path).getData(), true, 0);
                break;

            case JsonNodeType::JSON_NUMBER:
                jsonObjectPtr->createNumber("", newValue, getConcatenatedTraversalPath(path).getData(), true, 0);
                break;
        }
    }
    else if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        switch(type) {
            case JsonNodeType::JSON_OBJECT:
                jsonArrayPtr->createEmptyObject("", getConcatenatedTraversalPath(path).getData(), true, 0);
                break;

            case JsonNodeType::JSON_ARRAY:
                jsonArrayPtr->createEmptyArray("", getConcatenatedTraversalPath(path).getData(), true, 0);
                break;

            case JsonNodeType::JSON_STRING:
                jsonArrayPtr->createString("", newValue, getConcatenatedTraversalPath(path).getData(), true, 0);
                break;

            case JsonNodeType::JSON_KEYWORD:
                jsonArrayPtr->createKeyword("", newValue, getConcatenatedTraversalPath(path).getData(), true, 0);
                break;

            case JsonNodeType::JSON_NUMBER:
                jsonArrayPtr->createNumber("", newValue, getConcatenatedTraversalPath(path).getData(), true, 0);
                break;
        }
    }

    _changesHaveBeenMade = true;

    _undoData.addCreateUndoAction(getConcatenatedTraversalPath(path).getData());
}

void JsonParser::createInObject(const char* newKey, const char* newValue, const char* path, JsonNodeType type) {
    assertOpenFile();
    assertNonInitiatedMove();
    assertKey(newKey);

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        switch(type) {
            case JsonNodeType::JSON_OBJECT:
                jsonObjectPtr->createEmptyObject(newKey, getConcatenatedTraversalPath(path).getData(), false, 0);
                break;

            case JsonNodeType::JSON_ARRAY:
                jsonObjectPtr->createEmptyArray(newKey, getConcatenatedTraversalPath(path).getData(), false, 0);
                break;

            case JsonNodeType::JSON_STRING:
                jsonObjectPtr->createString(newKey, newValue, getConcatenatedTraversalPath(path).getData(), false, 0);
                break;

            case JsonNodeType::JSON_KEYWORD:
                jsonObjectPtr->createKeyword(newKey, newValue, getConcatenatedTraversalPath(path).getData(), false, 0);
                break;

            case JsonNodeType::JSON_NUMBER:
                jsonObjectPtr->createNumber(newKey, newValue, getConcatenatedTraversalPath(path).getData(), false, 0);
                break;
        }
    }
    else if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        switch(type) {
            case JsonNodeType::JSON_OBJECT:
                jsonArrayPtr->createEmptyObject(newKey, getConcatenatedTraversalPath(path).getData(), false, 0);
                break;

            case JsonNodeType::JSON_ARRAY:
                jsonArrayPtr->createEmptyArray(newKey, getConcatenatedTraversalPath(path).getData(), false, 0);
                break;

            case JsonNodeType::JSON_STRING:
                jsonArrayPtr->createString(newKey, newValue, getConcatenatedTraversalPath(path).getData(), false, 0);
                break;

            case JsonNodeType::JSON_KEYWORD:
                jsonArrayPtr->createKeyword(newKey, newValue, getConcatenatedTraversalPath(path).getData(), false, 0);
                break;

            case JsonNodeType::JSON_NUMBER:
                jsonArrayPtr->createNumber(newKey, newValue, getConcatenatedTraversalPath(path).getData(), false, 0);
                break;
        }
    }

    _changesHaveBeenMade = true;

    _undoData.addCreateUndoAction(addToPath(path, String(newKey)).getData());
}

void JsonParser::createEmptyObjectInArray(const char* path) {
    createInArray("", path, JsonNodeType::JSON_OBJECT);
}

void JsonParser::createEmptyObjectInObject(const char* newKey, const char* path) {
    createInObject(newKey, "", path, JsonNodeType::JSON_OBJECT);
}

void JsonParser::createEmptyArrayInArray(const char* path) {
    createInArray("", path, JsonNodeType::JSON_ARRAY);
}

void JsonParser::createEmptyArrayInObject(const char* newKey, const char* path) {
    createInObject(newKey, "", path, JsonNodeType::JSON_ARRAY);
}

void JsonParser::createStringInArray(const char* newStr, const char* path) {
    createInArray(newStr, path, JsonNodeType::JSON_STRING);
}

void JsonParser::createStringInObject(const char* newKey, const char* newStr, const char* path) {
    createInObject(newKey, newStr, path, JsonNodeType::JSON_STRING);
}

void JsonParser::createKeywordInArray(const char* newKeyword, const char* path) {
    createInArray(newKeyword, path, JsonNodeType::JSON_KEYWORD);
}

void JsonParser::createKeywordInObject(const char* newKey, const char* newKeyword, const char* path) {
    createInObject(newKey, newKeyword, path, JsonNodeType::JSON_KEYWORD);
}

void JsonParser::createNumberInArray(const char* newNumber, const char* path) {
    createInArray(newNumber, path, JsonNodeType::JSON_NUMBER);
}

void JsonParser::createNumberInObject(const char* newKey, const char* newNumber, const char* path) {
    createInObject(newKey, newNumber, path, JsonNodeType::JSON_NUMBER);
}

void JsonParser::moveFrom(const char* pathFrom) {
    assertOpenFile();

    if(pathFrom[0] == '\0') {
        throw std::invalid_argument("Invalid path for moving from");
    }

    String oldTraversedPath = _traversedPath;

    traverse(pathFrom);

    _moveFromPath = _traversedPath;
    _traversedPath = oldTraversedPath;
}

void JsonParser::moveToArray(const char* pathTo, const char* pathFrom) {
    assertOpenFile();

    if(pathFrom[0] != '\0' && !_moveFromPath.isEmpty()) {
        throw std::logic_error("Path to move from has already been given");
    }

    if(pathFrom[0] == '\0' && _moveFromPath.isEmpty()) {
        throw std::invalid_argument("Invalid path for moving from");
    }

    bool removedFromArray = false;
    JsonKey oldKey;

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        JsonPair jsonPairForMoving;

        if(!_moveFromPath.isEmpty()) {
            jsonPairForMoving = jsonObjectPtr->moveRemove(_moveFromPath.getData(), 0);
        }
        else {
            jsonPairForMoving = jsonObjectPtr->moveRemove(getConcatenatedTraversalPath(pathFrom).getData(), 0);
        }

        removedFromArray = strcmp(jsonPairForMoving.getKey().getData(), "") == 0;
        oldKey = jsonPairForMoving.getKey();

        try {
            jsonObjectPtr->move(std::move(jsonPairForMoving), getConcatenatedTraversalPath(pathTo).getData(), true, 0);
        }
        catch(const std::exception& exception) {

            if(!_moveFromPath.isEmpty()) {

                if(removedFromArray) {
                    _undoData.addRemoveUndoActionInArray(std::move(jsonPairForMoving.accessNode()), _moveFromPath.getData());
                }
                else {
                    _undoData.addRemoveUndoActionInObject(std::move(jsonPairForMoving), _moveFromPath.getData());
                }

            }
            else {

                if(removedFromArray) {
                    _undoData.addRemoveUndoActionInArray(std::move(jsonPairForMoving.accessNode()),
                                                         getConcatenatedTraversalPath(pathFrom).getData());
                }
                else {
                    _undoData.addRemoveUndoActionInObject(std::move(jsonPairForMoving),
                                                          getConcatenatedTraversalPath(pathFrom).getData());
                }
            }

            undo();
            throw;
        }
    }
    else if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        JsonPair jsonPairForMoving;

        if(!_moveFromPath.isEmpty()) {
            jsonPairForMoving = jsonArrayPtr->moveRemove(_moveFromPath.getData(), 0);
        }
        else {
            jsonPairForMoving = jsonArrayPtr->moveRemove(getConcatenatedTraversalPath(pathFrom).getData(), 0);
        }

        removedFromArray = strcmp(jsonPairForMoving.getKey().getData(), "") == 0;
        oldKey = jsonPairForMoving.getKey();

        try {
            jsonArrayPtr->move(std::move(jsonPairForMoving), getConcatenatedTraversalPath(pathTo).getData(), true, 0);
        }
        catch(const std::exception& exception) {

            if(!_moveFromPath.isEmpty()) {

                if(removedFromArray) {
                    _undoData.addRemoveUndoActionInArray(std::move(jsonPairForMoving.accessNode()), _moveFromPath.getData());
                }
                else {
                    _undoData.addRemoveUndoActionInObject(std::move(jsonPairForMoving), _moveFromPath.getData());
                }
            }
            else {

                if(removedFromArray) {
                    _undoData.addRemoveUndoActionInArray(std::move(jsonPairForMoving.accessNode()),
                                                         getConcatenatedTraversalPath(pathFrom).getData());
                }
                else {
                    _undoData.addRemoveUndoActionInObject(std::move(jsonPairForMoving),
                                                          getConcatenatedTraversalPath(pathFrom).getData());
                }
            }

            undo();
            throw;
        }
    }

    if(!_moveFromPath.isEmpty()) {

        if(removedFromArray) {
            _undoData.addMoveToArrayFromArrayUndoAction(getConcatenatedTraversalPath(pathTo).getData(), _moveFromPath.getData());
        }
        else {
            _undoData.addMoveToArrayFromObjectUndoAction(getConcatenatedTraversalPath(pathTo).getData(), _moveFromPath.getData(),
                                                         oldKey.getKeyString().getData());
        }
    }
    else {

        if(removedFromArray) {
            _undoData.addMoveToArrayFromArrayUndoAction(getConcatenatedTraversalPath(pathTo).getData(),
                                                        getConcatenatedTraversalPath(pathFrom).getData());
        }
        else {
            _undoData.addMoveToArrayFromObjectUndoAction(getConcatenatedTraversalPath(pathTo).getData(),
                                                         getConcatenatedTraversalPath(pathFrom).getData(),
                                                         oldKey.getKeyString().getData());
        }
    }


    _changesHaveBeenMade = true;
    _moveFromPath.clear();
}

void JsonParser::moveToObject(const char* pathTo, const char* pathFrom, const char* oldKey) {
    assertOpenFile();

    if(pathFrom[0] != '\0' && !_moveFromPath.isEmpty()) {
        throw std::logic_error("Path to move from has already been given");
    }

    if(pathFrom[0] == '\0' && _moveFromPath.isEmpty()) {
        throw std::invalid_argument("Invalid path for moving from");
    }

    bool removedFromArray = false;

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        JsonPair jsonPairForMoving;

        if(!_moveFromPath.isEmpty()) {
            jsonPairForMoving = jsonObjectPtr->moveRemove(_moveFromPath.getData(), 0);
        }
        else {
            jsonPairForMoving = jsonObjectPtr->moveRemove(getConcatenatedTraversalPath(pathFrom).getData(), 0);
        }

        removedFromArray = strcmp(jsonPairForMoving.getKey().getData(), "") == 0;

        if(strcmp(oldKey, "") != 0) {
            jsonPairForMoving.accessKey().accessKeyString() = oldKey;
        }

        try {
            jsonObjectPtr->move(std::move(jsonPairForMoving), getConcatenatedTraversalPath(pathTo).getData(), false, 0);
        }
        catch(const std::exception& exception) {

            if(!_moveFromPath.isEmpty()) {

                if(removedFromArray) {
                    _undoData.addRemoveUndoActionInArray(std::move(jsonPairForMoving.accessNode()), _moveFromPath.getData());
                }
                else {
                    _undoData.addRemoveUndoActionInObject(std::move(jsonPairForMoving), _moveFromPath.getData());
                }

            }
            else {

                if(removedFromArray) {
                    _undoData.addRemoveUndoActionInArray(std::move(jsonPairForMoving.accessNode()),
                                                         getConcatenatedTraversalPath(pathFrom).getData());
                }
                else {
                    _undoData.addRemoveUndoActionInObject(std::move(jsonPairForMoving),
                                                          getConcatenatedTraversalPath(pathFrom).getData());
                }
            }

            undo();
            throw;
        }
    }
    else if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        JsonPair jsonPairForMoving;

        if(!_moveFromPath.isEmpty()) {
            jsonPairForMoving = jsonArrayPtr->moveRemove(_moveFromPath.getData(), 0);
        }
        else {
            jsonPairForMoving = jsonArrayPtr->moveRemove(getConcatenatedTraversalPath(pathFrom).getData(), 0);
        }

        removedFromArray = strcmp(jsonPairForMoving.getKey().getData(), "") == 0;

        if(strcmp(oldKey, "") != 0) {
            jsonPairForMoving.accessKey().accessKeyString() = oldKey;
        }

        try {
            jsonArrayPtr->move(std::move(jsonPairForMoving), getConcatenatedTraversalPath(pathTo).getData(), false, 0);
        }
        catch(const std::exception& exception) {

            if(!_moveFromPath.isEmpty()) {

                if(removedFromArray) {
                    _undoData.addRemoveUndoActionInArray(std::move(jsonPairForMoving.accessNode()), _moveFromPath.getData());
                }
                else {
                    _undoData.addRemoveUndoActionInObject(std::move(jsonPairForMoving), _moveFromPath.getData());
                }
            }
            else {

                if(removedFromArray) {
                    _undoData.addRemoveUndoActionInArray(std::move(jsonPairForMoving.accessNode()),
                                                         getConcatenatedTraversalPath(pathFrom).getData());
                }
                else {
                    _undoData.addRemoveUndoActionInObject(std::move(jsonPairForMoving),
                                                          getConcatenatedTraversalPath(pathFrom).getData());
                }
            }

            undo();
            throw;
        }
    }

    if(!_moveFromPath.isEmpty()) {

        if(removedFromArray) {
            _undoData.addMoveToObjectFromArrayUndoAction(getConcatenatedTraversalPath(pathTo).getData(), _moveFromPath.getData());
        }
        else {
            _undoData.addMoveToObjectFromObjectUndoAction(getConcatenatedTraversalPath(pathTo).getData(),
                                                          _moveFromPath.getData(), "");
        }
    }
    else {

        if(removedFromArray) {
            _undoData.addMoveToObjectFromArrayUndoAction(getConcatenatedTraversalPath(pathTo).getData(),
                                                         getConcatenatedTraversalPath(pathFrom).getData());
        }
        else {
            _undoData.addMoveToObjectFromObjectUndoAction(getConcatenatedTraversalPath(pathTo).getData(),
                                                          getConcatenatedTraversalPath(pathFrom).getData(), "");
        }
    }

    _changesHaveBeenMade = true;
    _moveFromPath.clear();
}

void JsonParser::save(const char* fileName, const char* path) {
    assertOpenFile();
    assertJsonFileName(fileName);

    std::ofstream out(fileName);

    if(!out.is_open()) {
        throw std::ofstream::failure("Unable to open file");
    }

    if(_startingNodeType == JsonNodeType::JSON_OBJECT) {
        auto* jsonObjectPtr = (JsonObject*) _startingNode[0].get();

        jsonObjectPtr->savePath(out, getConcatenatedTraversalPath(path).getData(), 0);
    }
    else if(_startingNodeType == JsonNodeType::JSON_ARRAY) {
        auto* jsonArrayPtr = (JsonArray*) _startingNode[0].get();

        jsonArrayPtr->savePath(out, getConcatenatedTraversalPath(path).getData(), 0);
    }

    _changesHaveBeenMade = false;
}

void JsonParser::save(const char* path) {

    if(_changesHaveBeenMade || path[0] != '\0') {
        save( _fileName.getData(), path);
    }
}

void JsonParser::saveAs(const char* fileName, const char* path) {
    if(strcmp(fileName, _fileName.getData()) == 0) {
        throw std::invalid_argument("Given file name matches the one of the currently opened file");
    }

    save(fileName, path);
}
