#include "JsonArray.h"
#include "JsonObject.h"
#include "JsonValue.h"
#include "fileFunctions.h"

JsonArray::JsonArray(std::ifstream& in) {
    setData(in);
}

JsonArray::JsonArray(const JsonArray& other) {
    copyFrom(other);
}

JsonArray::JsonArray(JsonArray&& other) noexcept {
    moveFrom(std::move(other));
}

JsonArray& JsonArray::operator=(const JsonArray& other) {
    if(this != &other) {
        copyFrom(other);
        free();
    }

    return *this;
}

JsonArray& JsonArray::operator=(JsonArray&& other) noexcept {
    if(this != &other) {
        moveFrom(std::move(other));
        free();
    }

    return *this;
}

JsonArray::~JsonArray() {
    free();
}

void JsonArray::copyFrom(const JsonArray& other) {
    this->_jsonNodeCollection = other._jsonNodeCollection;

    for(unsigned i = 0; i < _jsonNodeCollection.getSize(); ++i) {
        this->_jsonNodeCollection[i] = other._jsonNodeCollection[i]->clone();
    }
}

void JsonArray::moveFrom(JsonArray&& other) {
    this->_jsonNodeCollection = other._jsonNodeCollection;

    other._jsonNodeCollection.clear();
}

void JsonArray::free() {

    for(unsigned i = 0; i < _jsonNodeCollection.getSize(); ++i) {
        delete _jsonNodeCollection[i];
    }
}

void JsonArray::setData(std::ifstream& in) {

    while(true) {
        char currentChar = (char)in.peek();

        if(currentChar == '\"' || currentChar == '-' || (currentChar >= '0' && currentChar <= '9')
           || currentChar == 't' || currentChar == 'f' || currentChar == 'n') {

            JsonValue matchingValue(in);
            _jsonNodeCollection.pushBack(matchingValue.clone());

        } else if(currentChar == '{') {

            in.get();

            JsonObject matchingObject(in);
            _jsonNodeCollection.pushBack(matchingObject.clone());

        } else if(currentChar == '[') {

            in.get();

            JsonArray matchingArray(in);
            _jsonNodeCollection.pushBack(matchingArray.clone());

        } else if(currentChar == ']') {

            return;

        } else {

            in.get();
        }

    }
}

JsonNode* JsonArray::clone() const {
    return new JsonArray(*this);
}
