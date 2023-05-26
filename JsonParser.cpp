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

    if(_startingNode.getTypeByIndex(0) == JsonNodeType::JSON_OBJECT
       ||_startingNode.getTypeByIndex(0) == JsonNodeType::JSON_ARRAY) {

        _startingNode[0]->search(searchResults, keyStr);
    }

    searchResults.print(0, false);
    std::cout << std::endl;
}
