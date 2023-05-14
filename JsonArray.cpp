#include "JsonArray.h"
#include "JsonObject.h"
#include "JsonValue.h"
#include "fileFunctions.h"

JsonArray::JsonArray(std::ifstream& in, unsigned nestingLevel) {
    _nestingLevel = nestingLevel;

    while(true) {
        char currentChar = (char)in.peek();

        if(currentChar == '\"') {

            JsonValue matchingValue(in, true);
            _jsonNodeCollection.pushBack(matchingValue.clone());

            currentChar = (char)in.peek();

            if(currentChar == ']') {
                in.get();
                return;
            }
        }

        if(currentChar == '-' || (currentChar >= '0' && currentChar <= '9')
           || currentChar == 't' || currentChar == 'f' || currentChar == 'n') {

            JsonValue matchingValue(in, false);
            _jsonNodeCollection.pushBack(matchingValue.clone());

            currentChar = (char)in.peek();

            if(currentChar == ']') {
                in.get();
                return;
            }
        }

        if(currentChar == '{') {
            in.get();

            JsonObject matchingObject(in, _nestingLevel + 1);
            _jsonNodeCollection.pushBack(matchingObject.clone());

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

            JsonArray matchingArray(in, _nestingLevel + 1);
            _jsonNodeCollection.pushBack(matchingArray.clone());

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

JsonArray::JsonArray(const JsonArray& other) : JsonNode(other), _jsonNodeCollection(other._jsonNodeCollection) {
    copyFrom(other);
}

JsonArray::JsonArray(JsonArray&& other) noexcept : JsonNode(other), _jsonNodeCollection(std::move(other._jsonNodeCollection)) {
    moveFrom(std::move(other));
}

JsonArray& JsonArray::operator=(const JsonArray& other) {
    if(this != &other) {
        JsonNode::operator=(other);
        _jsonNodeCollection = other._jsonNodeCollection;
        copyFrom(other);
        free();
    }

    return *this;
}

JsonArray& JsonArray::operator=(JsonArray&& other) noexcept {
    if(this != &other) {
        JsonNode::operator=(other);
        _jsonNodeCollection = std::move(other._jsonNodeCollection);
        moveFrom(std::move(other));
        free();
    }

    return *this;
}

JsonArray::~JsonArray() {
    free();
}

void JsonArray::copyFrom(const JsonArray& other) {
    this->_nestingLevel = other._nestingLevel;

    for(unsigned i = 0; i < _jsonNodeCollection.getSize(); ++i) {
        this->_jsonNodeCollection[i] = other._jsonNodeCollection[i]->clone();
    }
}

void JsonArray::moveFrom(JsonArray&& other) {
    this->_nestingLevel = other._nestingLevel;

    for(unsigned i = 0; i < _jsonNodeCollection.getSize(); ++i) {
        this->_jsonNodeCollection[i] = other._jsonNodeCollection[i];
        other._jsonNodeCollection[i] = nullptr;
    }
}

void JsonArray::free() {

    for(unsigned i = 0; i < _jsonNodeCollection.getSize(); ++i) {
        delete _jsonNodeCollection[i];
    }
}

void JsonArray::print() const {
    size_t numberOfElementsInArray = _jsonNodeCollection.getSize();

    std::cout << '[' << '\n';

    for(unsigned i = 0; i < numberOfElementsInArray; ++i) {

        printIndentation(_nestingLevel + 1);

        _jsonNodeCollection[i]->print();

        if(i == numberOfElementsInArray - 1) {
            std::cout << '\n';

            printIndentation(_nestingLevel);

            std::cout << ']';
            return;
        }

        std::cout << ',' << '\n';
    }
}

JsonNode* JsonArray::clone() const {
    return new JsonArray(*this);
}
