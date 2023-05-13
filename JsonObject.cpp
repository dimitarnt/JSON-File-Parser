#include "JsonObject.h"
#include "JsonValue.h"
#include "JsonArray.h"
#include "fileFunctions.h"

JsonObject::JsonObject(std::ifstream& in) {
    setData(in);
}

JsonObject::JsonObject(const JsonObject& other) {
    copyFrom(other);
}

JsonObject::JsonObject(JsonObject&& other) noexcept {
    moveFrom(std::move(other));
}

JsonObject& JsonObject::operator=(const JsonObject& other) {
    if(this != &other) {
        copyFrom(other);
        free();
    }

    return *this;
}

JsonObject& JsonObject::operator=(JsonObject&& other) noexcept {
    if(this != &other) {
        moveFrom(std::move(other));
        free();
    }

    return *this;
}

JsonObject::~JsonObject() {
    free();
}

void JsonObject::copyFrom(const JsonObject& other) {
    this->_jsonNodeCollection = other._jsonNodeCollection;
    this->_correspondingKeys = other._correspondingKeys;

    for(unsigned i = 0; i < _jsonNodeCollection.getSize(); ++i) {
        this->_jsonNodeCollection[i] = other._jsonNodeCollection[i]->clone();
    }
}

void JsonObject::moveFrom(JsonObject&& other) {
    this->_jsonNodeCollection = other._jsonNodeCollection;
    this->_correspondingKeys = other._correspondingKeys;

    other._jsonNodeCollection.clear();
    other._correspondingKeys.clear();
}

void JsonObject::free() {

    for(unsigned i = 0; i < _jsonNodeCollection.getSize(); ++i) {
        delete _jsonNodeCollection[i];
    }
}

void JsonObject::setData(std::ifstream& in) {

    while(true) {
        while(in.peek() != '\"') {
            in.get();
        }

        _correspondingKeys.pushBack(parseValue(in));

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

            } else if(currentChar == '}') {

                return;

            } else {

                in.get();
            }
        }
    }
}

JsonNode* JsonObject::clone() const {
    return new JsonObject(*this);
}
