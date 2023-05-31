#include "JsonParser.h"
#include "JsonArray.h"
#include "InvalidJsonSyntax.h"
#include "constants.h"
#include <cstring>

void JsonParser::assertJsonFileName(const char* fileName) {
    String name(fileName);
    String extension(JSON_FILE_EXTENSION);
    size_t extensionLength = extension.getLength();

    if(name.substring(name.getLength() - extensionLength, extensionLength) != extension) {
        throw std::invalid_argument("File name does not have a JSON file extension");
    }
}

JsonParser::JsonParser(const char* fileName) {
    setFile(fileName);
}

void JsonParser::setFile(const char* fileName) {
    assertJsonFileName(fileName);

    std::ifstream in(fileName);

    if(!in.is_open()) {
        throw std::ifstream::failure("Unable to open file");
    }

    JsonValidator fileValidator(in);

    fileValidator.validate();

    char firstChar = (char)in.get();

    if(firstChar == '{') {
        _startingNode.addJsonNode(JsonNodeType::JSON_OBJECT, in);
    }

    if(firstChar == '[') {
        _startingNode.addJsonNode(JsonNodeType::JSON_ARRAY, in);
    }
}

void JsonParser::print() const {
    _startingNode[0]->print(0, false);
    std::cout << std::endl;
}

void JsonParser::search(const char* key) const {
    JsonArray searchResults;
    String keyStr(key);

    _startingNode[0]->search(searchResults, keyStr);

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
    assertString(newStr);

    _startingNode[0]->set(path, newStr, 0);
}

void JsonParser::createInArray(const char* path, const char* newStr) {
    create(path, false, true, "", newStr);
}

void JsonParser::createInObject(const char* newKey, const char* newStr) {
    create("", true, false, newKey, newStr);
}

void JsonParser::createInObject(const char* path, const char* newKey, const char* newStr) {
    create(path, false, false, newKey, newStr);
}

void JsonParser::create(const char* path, bool isAddressingStartingNode, bool createInArray, const char* newKey, const char* newStr) {
    _startingNode[0]->create(path, isAddressingStartingNode, createInArray, newKey, newStr, 0);
}

void JsonParser::remove(const char* path) {
    _startingNode[0]->remove(path, 0);
}
