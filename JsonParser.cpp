#include "JsonParser.h"
#include "JsonObject.h"
#include "JsonArray.h"
#include "constants.h"

void JsonParser::assertJsonFileName(const char* fileName) const {
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
        JsonObject startingObject(in, 1);
        _startingNode = startingObject.clone();
    }

    if(firstChar == '[') {
        JsonArray startingArray(in, 1);
        _startingNode = startingArray.clone();
    }
}

void JsonParser::print() const {
    _startingNode->print();
}
