#include "JsonParser.h"
#include "JsonObject.h"
#include "JsonArray.h"
#include "constants.h"

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

JsonParser::JsonParser(const JsonParser& other) : _fileName(other._fileName) {
    copyFrom(other);
}

JsonParser::JsonParser(JsonParser&& other) noexcept : _fileName(std::move(other._fileName)) {
    moveFrom(std::move(other));
}

JsonParser& JsonParser::operator=(const JsonParser& other) {
    if(this != &other) {
        _fileName = other._fileName;
        copyFrom(other);
        free();
    }

    return *this;
}

JsonParser& JsonParser::operator=(JsonParser&& other) noexcept {
    if(this != &other) {
        _fileName = std::move(other._fileName);
        moveFrom(std::move(other));
        free();
    }

    return *this;
}

JsonParser::~JsonParser() {
    free();
}

void JsonParser::copyFrom(const JsonParser& other) {
    _startingNode = other._startingNode->clone();
}

void JsonParser::moveFrom(JsonParser&& other) {
    _startingNode = other._startingNode;
    other._startingNode = nullptr;
}

void JsonParser::free() {
    delete _startingNode;
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
        JsonObject startingObject(in, 0);
        _startingNode = startingObject.clone();
    }

    if(firstChar == '[') {
        JsonArray startingArray(in, 0);
        _startingNode = startingArray.clone();
    }
}

void JsonParser::print() const {
    _startingNode->print();
}
